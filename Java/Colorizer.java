/**
 * Timothy Pratt
 * Takes pixel array from PPMFile and applies recoloring with threads
 */

import java.io.*;
public class Colorizer implements Runnable
{
    private Pixel[] pixels;

    public Colorizer(Pixel[] pixels)
    {
		this.pixels = pixels;
	}

    public void run()
    {
        for (int i = 0; i < pixels.length; i++)
        {
            Pixel pix = this.pixels[i];
            int avg = (pix.r + pix.g + pix.g) / 3;
            pix.r = avg;
            pix.g = avg;
            pix.b = avg;
		}
	}
    public static void main(String[] args)
    {
        PPMFile ppm = new PPMFile();
        String inputFile = "IMG_4203.ppm";
        String outputFile = "output.ppm";
 
        try
        {
            ppm.readFile(inputFile);
            Pixel[] p = ppm.getPixels();
            // Pixel[] p1 = new Pixel[p.length];
            // Pixel[] p1 = new Pixel[p.length/2];
            // Pixel[] p2 = new Pixel[p.length/2];
            Pixel[] p1 = new Pixel[p.length/4];
            Pixel[] p2 = new Pixel[p.length/4];
            Pixel[] p3 = new Pixel[p.length/4];
            Pixel[] p4 = new Pixel[p.length/4];

            for (int i = 0; i < p.length; i++)
            {
                // p1[i] = p[i];
                
                // if (i < p.length/2)
                // {
                //     p1[i] = p[i];
                // }
                // else
                // {
                //     p2[i - p.length/2] = p[i];
                // }

                if (i < p.length/4)
                {
                    p1[i] = p[i];
                }
                else if (i < p.length/2)
                {
                    p2[i - p.length/4] = p[i];
                }
                else if (i < 3 * p.length/4)
                {
                    p3[i - p.length/2] = p[i];
                }
                else
                {
                    p4[i - (3 * p.length)/4] = p[i];
                }
            }

            Colorizer c1 = new Colorizer(p1);
            Colorizer c2 = new Colorizer(p2);
            Colorizer c3 = new Colorizer(p3);
            Colorizer c4 = new Colorizer(p4);             
            Thread t1 = new Thread(c1);
            Thread t2 = new Thread(c2);
            Thread t3 = new Thread(c3);
            Thread t4 = new Thread(c4);
            long start = System.currentTimeMillis();
            t1.start();
            t2.start();
            t3.start();
            t4.start();

            try
            {
                t1.join();
                t2.join();
                t3.join();
                t4.join();
            }
            
            catch (Exception e)
            {
                System.err.println(e);
            }

            long stop = System.currentTimeMillis();
            System.out.println("PPM file processed successfully in " + (stop-start) + " milliseconds" );

            for (int i = 0; i < p.length; i++)
            {
                // p[i] = c1.pixels[i];

                // if (i < p.length/2)
                // {
                //     p[i] = c1.pixels[i];
                // }
                // else
                // {
                //     p[i] = c2.pixels[i - p.length/2];
                // }

                if (i < p.length/4)
                {
                    p[i] = c1.pixels[i];
                }
                else if (i < p.length/2)
                {
                    p[i] = c2.pixels[i - p.length/4];
                }
                else if (i < 3 * p.length/4)
                {
                    p[i] = c3.pixels[i - p.length/2];
                }
                else
                {
                    p[i] = c4.pixels[i - (3 * p.length/4)];
                }
            }

            ppm.setPixels(p);
            ppm.writeFile(outputFile);
        }
        
        catch (IOException e)
        {
            System.out.println("Error processing PPM file: " + e.getMessage());
        }
    }
}