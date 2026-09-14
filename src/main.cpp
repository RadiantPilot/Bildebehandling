#include <cstdint>
#include <fstream>
#include <cmath>
#include <iostream>
#include <filesystem>
#include "image.hpp"
#include "utils.hpp"


void convert(const std::string& INPUT, const std::string& OUTPUT)
{
    const std::filesystem::path input_path(INPUT);
    for(auto& file : std::filesystem::directory_iterator{input_path}) {
        if(!file.is_directory()) {
            auto in_filename = file.path().filename().string();
            auto out_filename = file.path().filename().stem().string()+".pgm";
            std::cout << "Reading " << in_filename << '\n';
            imglib::image im(INPUT+in_filename);
            std::cout << "Writing " << out_filename << '\n';
            im.save(OUTPUT+out_filename);
        }
    }
}
// convert(INPUT, OUTPUT);


imglib::image regnUtBilder(imglib::image f1, imglib::image f2, char op)
{
    unsigned int N = std::min(f1.width(),   f2.width());
    unsigned int M = std::min(f1.height(),  f2.height());

    imglib::image g(M, N);

    for(unsigned int i = 0; i < M; ++i){
        for(unsigned int j = 0; j < N; ++j){
            double a = f1.begin()[i * f1.width() + j];
            double b = f2.begin()[i * f2.width() + j];
            double results;

            switch (op)
            {
                case '+': results = a + b; break;
                case '-': results = a - b; break;
                case '*': results = a * b; break;
                case '/': results = a / b * 255; break;
                default: results = 0; break;
            }

            g.begin()[i * N + j] = results;
        }
    }

    return g;

}

imglib::image normaliserBilde(imglib::image f)
{
    unsigned int N = f.width();
    unsigned int M = f.height();

    imglib::image g(M, N);

    // Finner piksel med lavest og høyest lysverdi
    double minVerdi = f.begin()[0];
    double maxVerdi = f.begin()[0];

    for (unsigned int i = 0; i < M * N; i++)
    {
        double verdi = f.begin()[i];
        if (verdi < minVerdi) minVerdi = verdi;
        if (verdi > maxVerdi) maxVerdi = verdi;
    }

    double spennvidde = maxVerdi - minVerdi;
    if (spennvidde == 0) spennvidde = 1;

    for (unsigned int i = 0; i < M * N; i++)
    {
        g.begin()[i] = (f.begin()[i] - minVerdi) / spennvidde * 255;
    }

    return g;
}


imglib::image Oppg_3_a(imglib::image f, double scale)
// Scaling with nearest neighbour
{
    unsigned int M = f.height();
    unsigned int N = f.width();

    unsigned int New_Height = (unsigned int)(M * scale);
    unsigned int New_Width  = (unsigned int)(N * scale);

    imglib::image g(New_Height, New_Width);

    for(unsigned int i = 0; i < New_Height; i++)
    {
        for(unsigned int j = 0; j < New_Width; j++)
        {
            unsigned int round_i = (unsigned int)(i / scale);
            unsigned int round_j = (unsigned int)(j / scale);

            g.begin()[i * New_Width + j] = f.begin()[round_i * N + round_j];
        }
    }
    return g;
}


imglib::image Oppg_3_b(imglib::image f, double scale)
// Scaling with bilinear interpolation
{
    unsigned int M = f.height();
    unsigned int N = f.width();

    unsigned int New_Height = (unsigned int)(M * scale);
    unsigned int New_Width  = (unsigned int)(N * scale);

    imglib::image g(New_Height, New_Width);

    // Henter pikser (r, c) fra f, men kun i gyldig område. Gjør at vi aldri leser utenfor bildet.
    auto hent = [&](unsigned int r, unsigned int c) -> double {
        if (r >= M) r = M - 1;
        if (c >= N) c = N - 1;
        return f.begin()[r * N + c];
    };

    double dx = (double)(New_Height - 1) / (M - 1);
    double dy = (double)(New_Width  - 1) / (N - 1);

    for(unsigned int i = 0; i < New_Height; i++)
    {
        for(unsigned int j = 0; j < New_Width; j++)
        {
            unsigned int x1 = (unsigned int)(i / dx);
            unsigned int y1 = (unsigned int)(j / dy);

            double x = i / dx - x1;
            double y = j / dy - y1;

            double a = hent(x1+1, y1)     - hent(x1, y1);
            double b = hent(x1,   y1+1)   - hent(x1, y1);
            double c = hent(x1+1, y1+1)   + hent(x1, y1) - hent(x1+1, y1) - hent(x1, y1+1);
            double d = hent(x1,   y1);

            g.begin()[i * New_Width + j] = a*x + b*y + c*x*y + d;
        }
    }
    return g;
}

// Rotation of image
imglib::image Oppg_4(imglib::image f, float rotation)
{
    int M = (int)f.height();
    int N = (int)f.width();

    imglib::image g(M, N);

    // Gjør om fra grader til radianer
    const double pi = 3.1415;
    double theta = rotation * pi / 180;
    double cosT = std::cos(theta);
    double sinT = std::sin(theta);

    // Finner midtpunktet i bildet
    double ci = (M - 1) / 2.0;
    double cj = (N - 1) / 2.0;
    
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // 1.   Flytter origo til midten
            double y = i - ci;
            double x = j - cj;

            // 2.   Roterer baklengs (for å finne hvor pikselene kom fra)
            double x_kilde = cosT * x + sinT * y;
            double y_kilde = -sinT * x + cosT * y;

            // 3.   Flytter origo tilbake (pluss avrunding til nærmeste piksel)
            int r = (int)std::round(y_kilde + ci);
            int c = (int)std::round(x_kilde + cj);

            // 4. Kopierer pikselen hvis den ligger inne i originalbildet. 
            // Hvis den ikke gjør det, setter den pikselen til svart.
            if (r >= 0 && r < M && c >= 0 && c < N)
            {
                g.begin()[i * N + j] = f.begin()[r * N + c];
            }
            else
            {
                g.begin()[i * N + j] = 0;
            }
        }
    }
    return g;
}



int main()
{
    const std::string INPUT = "images/input/";
    const std::string OUTPUT = "images/output/";

    // Oppg 1b: Åpner en fil som ikke finnes, og lagrer den som empty.bmp
    imglib::image tom("image/input/finnes_ikke.bmp");
    tom.save(OUTPUT + "empty.bmp");

    imglib::image im_1(INPUT + "Armstrong.bmp");
    imglib::image im_2(INPUT + "Untethered.bmp");

    //Oppg 2: Kjører bildeoperasjonene
    imglib::image sum = regnUtBilder(im_1, im_2, '+');    // + kan byttes ut med den operasjonen du vil bruke (+, -, *, /)
    sum.save(OUTPUT + "oppg_2_sum.bmp");

    imglib::image normalisert =normaliserBilde(im_1);
    normalisert.save(OUTPUT + "oppg_2e_normalisert.bmp");

    // Oppg 3:
    imglib::image skalert_a = Oppg_3_a(im_1, 2.0);
    skalert_a.save(OUTPUT + "oppg_3a_skalert.bmp");

    imglib::image skalert_b = Oppg_3_b(im_2, 2.0);
    skalert_b.save(OUTPUT + "oppg_3b_skalert.bmp");

    // Oppg 4:
    imglib::image rotert = Oppg_4(im_1, 30);
    rotert.save(OUTPUT + "oppg_4_rotert.bmp");

    return 0;
}
