Image oilPaint(Image& image) {
    const int radius = 3;
    const int levels = 20;
    Image result(image.width, image.height);

    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {

            int count[levels] = {0};
            int sumR[levels] = {0};
            int sumG[levels] = {0};
            int sumB[levels] = {0};

            for (int dx = -radius; dx <= radius; dx++) {
                for (int dy = -radius; dy <= radius; dy++) {
                    int nx = i + dx;
                    int ny = j + dy;

                    if (nx < 0 || ny < 0 || nx >= image.width || ny >= image.height)
                        continue;

                    int R = image(nx, ny, 0);
                    int G = image(nx, ny, 1);
                    int B = image(nx, ny, 2);

                    int intensity = (R + G + B) / 3;
                    int level = intensity * levels / 256;

                    count[level]++;
                    sumR[level] += R;
                    sumG[level] += G;
                    sumB[level] += B;
                }
            }

            int best = 0;
            for (int l = 1; l < levels; l++) {
                if (count[l] > count[best]) best = l;
            }

            result(i, j, 0) = sumR[best] / count[best];
            result(i, j, 1) = sumG[best] / count[best];
            result(i, j, 2) = sumB[best] / count[best];
        }
    }
    return result;
}