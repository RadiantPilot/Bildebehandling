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



void oppg_2()
{
    const std::string INPUT = "images/input/";
    const std::string OUTPUT = "images/output/";

    imglib::image im(INPUT + "Untethered.bmp");
    imglib::image im2(INPUT + "ARMSTRONG.bmp");

    imglib::image result(im.height(), im.width());

    int H = std::min(im.height(),  im2.height());
    int W = std::min(im.width(),  im2.width());

    // std::cout << im.height() << "," << im.width() << '\n';

    for(int i = 0; i < H; ++i){
        for(int j = 0; j < W; ++j){
            result(i,j) = (im(i,j) / im2(i,j)) * 255;   // Bytt ut / med +, - eller *
        }
    }

    result.save(OUTPUT+"Innlevering_1_Oppg_2.bmp");
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


int main()
{
    const std::string INPUT = "images/input/";
    const std::string OUTPUT = "images/output/";

    // Oppg 1b: Åpner en fil som ikke finnes, og lagrer den som empty.bmp
    imglib::image tom("image/input/finnes_ikke.bmp");
    tom.save(OUTPUT + "empty.bmp");

    //Oppg 2: Kjører bildeoperasjonene
    oppg_2();

    imglib::image im(INPUT + "Untethered.bmp");
    imglib::image skalert = Oppg_3_b(im, 2.0);
    skalert.save(OUTPUT + "oppg_3b_skalert.bmp");

    return 0;
}
