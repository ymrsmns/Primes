package mayton.primes;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.FileOutputStream;

import static java.lang.String.format;

/**
 * 5.04.2015 - mayton - In beginning...
 *
 * @author mayton
 */
public class Histogram {

    public static void printf(String s, Object... arg) {
        System.out.printf(s, arg);
    }

    public static void err(String s, Object... arg) {
        System.err.printf(s, arg);
    }

    public static void setRect(BufferedImage i, int x, int y, int size, int color) {
        if ((x >= 0) && (y >= 0) && (x + size < i.getWidth()) && (y + size < i.getHeight())) {
            for (int xx = x; xx < x + size; xx++) {
                for (int yy = y; yy < y + size; yy++) {
                    i.setRGB(xx, yy, color);
                }
            }
        }
    }

    public static void main(String[] args) throws Exception {
        int end = 1000000;
        int primes = 1;
        final int WIDTH = 1320;             // 800 = 60 * 13
        final int HEIGHT = 1 + end / WIDTH;// HEIGHT = 100 000 / 800 + 1
        final int SNAP = 60;               // Column grid size
        BufferedImage image = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_ARGB);
        for (int x = 0; x < WIDTH; x++) {
            if (x % (SNAP * 2) < SNAP) {
                for (int y = 0; y < HEIGHT; y++) {
                    image.setRGB(x, y, Color.WHITE.getRGB());
                }
            } else {
                for (int y = 0; y < HEIGHT; y++) {
                    image.setRGB(x, y, Color.YELLOW.getRGB());
                }
            }
        }

        for (int c = 3; c <= end; c += 2) {
            int ub = 1 + (int) Math.sqrt((double) c);
            boolean isprime = true;
            for (int i = 2; i <= ub; i++) {
                if (c % i == 0) {
                    isprime = false;
                    break;
                }
            }
            if (isprime) {
                primes++;
                int x = (c % WIDTH);
                int y = (c / WIDTH);
                image.setRGB(x, y, Color.BLACK.getRGB());
            }
        }
        ImageIO.write(image, "png", new FileOutputStream(format("primes_distr_%sx%s.png", WIDTH, HEIGHT)));
    }

}
