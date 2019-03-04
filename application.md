# Application 

## Embarrassingly Computations

A computation can be divided into a number of completely **independent** tasks

- Image Transformations

  - Low-level image opreations:

    - Shifting: object shifted by $\Delta x$ in the x-dimension and $\Delta y$ in the y-dimension
    - Scaling: object scaled by a factor of $Sx$ in the x-direction and $Sy$ in the y-direction
    - Rotation: object rotated through the angle $\theta$ about the origin of the coordinate system

  - Partition

    Square region, row region, column region

- Monte Carlo Methods

  A class of computational algorithms that rely on repeated random sampling to compute their results

  ($\pi$ calculation, Integration)

- Mandelbrot set

  A set of complex numbers that are quasi-stable when computed by iterating the function

  $Z_0 = C, Z_{k+1} = Z_{k}^2+C$

  - C is some complex number: $C = a + bi$

  - $Z_{k+1}$ is the $(k+1)^{\text{th}}$ iteration of the complex number

  - If $|Z_k|<=2$ for ANY $k\rightarrow C$ belongs to Mandelbrot set

  - It is fractal: An object that display self-simiarity at various scale, magiifying a fractal reveals small-scale details similar to the large-scale characteristics

  - Compute $Z_{k+1} = Z_{k}^2 + C$

    Let $C = C_{real} + C_{imag}i , Z_k = Z_{real} + Z_{imag} i$
    $$
    Z_{real\_next} = Z_{real} ^ 2 - Z_{imag}^2 + C_{real} \notag \\
    Z_{imag\_next} = 2Z_{real} Z_{imag} + C_{imag}
    $$

  - Represent image number in program

    ```C
    Struct complex{
        float real;
        float imag
    };
    ```

  - ```c
    int cal_pixel(complex c)
    {
        int count = 0; 
        int max = 256;
        float temp, lengthsq;
        complex z;
        z.real = 0; z.imag=0;
        do {
            temp = (z.real * z.real) - (z.imag*z.imag) + c.real;
            z_imag = (2*z.real*z.imag) + c.imag;
            z.real = temp;
            lengthsq = (z.real*z.real) + (z.imag*z.imag);
            count++;
        } while((lengthsq<4.0) && (count<max));
        return count;
    }
    ```

    ```C
    for(x = real_min; x < real_max; x+= real_dist)
        for(y=imag_min; y<imag_max; x+=imag_dist)
        {
            c.real =x; c.imag=y;
            color = cal_pixel(c);
            display(x,y,color);
        }
    ```

  - Parallzling Mandelbrot Set program

    (Each process may have different load)

## Load Balancing & Termination



## Divide-And-Conquer Computations





## Pipelilned Computations



## Synchronous Computations

