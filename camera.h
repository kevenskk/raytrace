#ifndef CAMERA_H
#define CAMERA_H

#include "helper.h"
#include "ray.h"
#include "colour.h"
#include "hittableobjects.h"
#include <chrono>
#include "material.h"
#include <SDL2/SDL.h>
#include <bits/stdc++.h> 
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"


using namespace std;


bool running = true;


SDL_Event event;

bool rendered;
const char *title = "Kevin Ray Tracer";

class Camera {
  public:
    Camera(SDL_Window* window, SDL_Renderer* renderer) : window(window), renderer(renderer) {}

    SDL_Renderer *renderer;
    SDL_Window *window;


    double aspectRatio;  // aspect ratio of the image
    int    imgwidth; 
    int imgheight;  
    int    spp;  // number of random samples/camera rays for each pixel
    int bounces;  //  number of bounces for a camera ray
    colour bg;
    int fov;

    
    point3 cameraPosition = point3(0, 0, -1); // position of camera
    point3 cameraLookAt = point3(0, 0, 0); // direction camera is looking at
    vec3 cameraUp = vec3(0, 1, 0); // up direction of camera
    
    
    void input() { // handle SDL events
      while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);


        if (event.type == SDL_QUIT) {
          running = false;
         
          SDL_DestroyRenderer(renderer);
          SDL_DestroyWindow(window);
          SDL_Quit();
          exit(0);
        } else if (event.type == SDL_WINDOWEVENT) {
          switch (event.window.event) {
            case SDL_WINDOWEVENT_MOVED:
              std::clog << "\n Window moved" << event.window.data1 << ", " << event.window.data2 << "\n";
              break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
              std::clog << "\n Window lost focus\n";
              break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
              std::clog << "\n Window gained focus\n";
              break;
          }
        }
      }
    }

   
    void render(const hittable& scene) {
        
        initialize();
        SDL_SetWindowSize(window, imgwidth, imgheight);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        
           
        std::cout << "P3\n" << imgwidth << ' ' << imgheight << "\n255\n";
        auto start = std::chrono::high_resolution_clock::now();
        rendered = false;
        for (int j = 0; j < imgheight; ++j) {
            std::clog << "\r Rows remaining: " << (imgheight - j) << ' ' << std::flush;
            
               
            for (int i = 0; i < imgwidth; ++i) {
                 colour pixelColour(0,0,0);
                for (int s = 0; s < spp; ++s) {
                    ray r = getRay(i, j);
                    pixelColour += getRayColour(r,bounces, scene);

                  
                }


                
                auto r = pixelColour.r();
                auto g = pixelColour.g();
                auto b = pixelColour.b();

                 
                
                 const float scale = 1.0 / spp; 
                     r = sqrt(r*scale); 
                     g = sqrt(g*scale);
                     b = sqrt(b*scale);
  
               

                static const interval rgb(0.000, 0.999); 
                SDL_SetRenderDrawColor(renderer, static_cast<int>(256 * rgb.clamp(r)), static_cast<int>(256 * rgb.clamp(g)), static_cast<int>(256 * rgb.clamp(b)), 255);       
                SDL_RenderDrawPoint(renderer, i, j);
                


                input();
                drawColour(std::cout, pixelColour, spp);
               
                
                // draw to sdl window
                if(i % imgwidth == 1)  {
                  SDL_RenderPresent(renderer);
                  

                }
                
            
                
                  
                
                
               
               
            }
            
        }
        
      
        // execute after render has completed
        
        
        rendered = true;
        auto end = std::chrono::high_resolution_clock::now();

        auto rendertime = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

        std::clog << "\nRender time: " << rendertime.count() << " seconds\n";

        while(rendered == true){

          input();
        }
      
        
   
  
      
      
       
      

}

  private:
    vec3 u,v,w;
    point3 cameraCentre; 
    point3 pixelUpperLeft;   
    vec3   pixelDeltaU;  
    vec3   pixelDeltaV;  
    

    void initialize() {
        imgheight = static_cast<int>(imgwidth / aspectRatio);
        imgheight = (imgheight < 1) ? 1 : imgheight;

        cameraCentre = cameraPosition;
      
        // viewport
        auto focalLength = (cameraPosition - cameraLookAt).length();
        auto theta = fov * pi / 180.0;
        auto h =  tan(theta/2);
        
        auto viewportHEIGHT = 2.0 * h * focalLength;
        auto viewportWidth = viewportHEIGHT * (static_cast<double>(imgwidth)/imgheight);

          w = normalize(cameraPosition - cameraLookAt); 
          u = normalize(cross(cameraUp, w));
          v = cross(w, u);

        vec3 viewportU = viewportWidth * u;
        vec3 viewportV = viewportHEIGHT * -v;

        pixelDeltaU = viewportU/ imgwidth;
        pixelDeltaV = viewportV / imgheight;

        auto viewportUpperLeft = cameraCentre -  (focalLength * w) - viewportU/2 - viewportV/2;
        pixelUpperLeft = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
    }
        
     ray getRay(int i, int j) const {

        auto pixelCentre= pixelUpperLeft + (i * pixelDeltaU) + (j * pixelDeltaV);
        auto pixelSample= pixelCentre + randomPSample();

        auto rayOrigin = cameraCentre;
        auto rayDirection = pixelSample - cameraCentre; // direction from ray origin to randomly generated pixel sample
        return ray(rayOrigin, rayDirection); // return ray that points to random sample
    }

    vec3 randomPSample() const { 
        // generate a random sample in the pixel square
        auto px = -0.5 + RandomDouble();
        auto py = -0.5 + RandomDouble();
        return (px * pixelDeltaU) + (py * pixelDeltaV);
    }

   colour getRayColour(const ray& r, int bounces, const hittable& scene) {
            hitRecord rec;

     if (bounces<= 0)
            return colour(0,0,0);


        if (!scene.hit(r, interval(0.00001, infinity), rec)) {
        vec3 nDir = normalize(r.direction()); // return sky if no hit
        auto offset = 0.5*(nDir.y() + 1.0);
        return (1.0-offset)*colour(1.0, 1.0, 1.0) + offset*colour(0.357, 0.608, 0.988);
        }

         
        ray scattered;
        colour attenuation;
        colour lightColour = rec.m->emitted(rec.u, rec.v, rec.p);

        if (!rec.m->scatter(r, rec, attenuation, scattered))
            return lightColour;

        colour accumulatedColour = attenuation * getRayColour(scattered, bounces-1, scene);

        return lightColour + accumulatedColour;

         
        
        
        

       
    }
};
#endif