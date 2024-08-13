#include "helper.h"

#include "camera.h"
#include "hittablelist.h"
#include "sphere.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL2/SDL.h>
#include <chrono>
#include <thread> 




    hittableList scene;




int main(int argc, char* argv[]) {
    
    

    // materials 
    auto ground = make_shared<diffuse>(colour(1,1,1));
    auto matte = make_shared<diffuse>(colour(0.7, 0.7, 0.5));
    auto black = make_shared<diffuse>(colour(0,0,0));


    auto surfaceTexture = make_shared<surfacetexture>("foam-blue-water.jpg"); // https://www.freepik.com/free-photo/foam-blue-water_2209780.htm#query=high%20resolution%20texture&position=40&from_view=keyword&track=ais&uuid=4d7dec64-c14c-4cd2-be48-f485c226418b
    auto surface = make_shared<diffuse>(surfaceTexture);



    auto polishedMetal  = make_shared<metal>(colour(0.6, 0.6, 0.5), 0.0); // metal with no roughness
    auto roughMetal = make_shared<metal>(colour(0.8, 0.6, 0.8), 0.2); // metal material with some defined roughness
    auto sphericallight = make_shared<emissive>(colour(4, 4, 4)); // light source
    
    auto tintedglass = make_shared<refractive>(colour(0.647, 0.89, 0.804),1.0, 1.5); 
     auto glass = make_shared<refractive>(colour(1,1,1),1.0, 1.5); 

   
   
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);  
    Camera camera(window, renderer);
      
   

    
     // camera parameters
    camera.aspectRatio = 16.0 / 9.0;
    camera.imgwidth  = 800;
    camera.spp = 100;
    camera.bounces = 50;
    camera.bg = colour(0, 0, 0);
    camera.fov = 90;
    camera.cameraPosition = point3(-2,2,1);
    camera.cameraLookAt   = point3(0,0,-1);
    camera.cameraUp     = vec3(0,1,0);
    


        

  

      
   

    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext(); 
    ImGuiIO& io = ImGui::GetIO(); (void)io;


    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer); 
    ImGui::StyleColorsDark();
    ImGui_ImplSDLRenderer2_Init(renderer);
   

    bool running = true;
    while(running){
     SDL_Event e;
    while (SDL_PollEvent(&e)) {
        ImGui_ImplSDL2_ProcessEvent(&e);


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
    
    
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
   
       

    ImGui::Begin("Render");
    
   
    if (ImGui::Button("Start Rendering")) {
      camera.render(scene);

    }

   
   

    ImGui::InputInt("Samples", &camera.spp);
    ImGui::InputInt("Image Width", &camera.imgwidth);


    ImGui::SliderInt("Bounces", &camera.bounces, 1, 100);


       
    
   
    
        
   


    shared_ptr<material> mat;


    ImGui::End();
    const char* scenes[] = {"Scene 1", "Scene 2"};

    static int currentScene = 0;

    if (ImGui::BeginCombo("Scene", scenes[currentScene])) {
    for (int n = 0; n < IM_ARRAYSIZE(scenes); n++) {
        bool isSelected = (currentScene == n);
        if (ImGui::Selectable(scenes[n], isSelected)) {
            currentScene = n;

            switch (currentScene) {


               
                case 0:
                      camera.fov = 40;

                      camera.cameraPosition = point3(-2,2,1);
                      camera.cameraLookAt = point3(0,0,-1);
                      camera.cameraUp = vec3(0,1,0);
                      scene.add(make_shared<sphere>(point3( 0, 7, 0),   3, sphericallight));
                      scene.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, ground));
                      scene.add(make_shared<sphere>(point3( 0.0, 0.0 , -1),  0.5, glass));
                      scene.add(make_shared<sphere>(point3( 0.0, 0.0 , -1),  -0.48, glass));
                      scene.add(make_shared<sphere>(point3(1.0,    0.0, -1.0),   0.5, surface));
                      scene.add(make_shared<sphere>(point3(2.0,    0.0, -1.0), 0.5, polishedMetal));

                      scene.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, matte));
                      scene.add(make_shared<sphere>(point3(-2.0,    0.0, -1.0),   0.5, roughMetal)); 
                      

                      
                     
                    break;
                case 1:
                 camera.fov = 40;
                 camera.aspectRatio = 4/3;
                 camera.cameraPosition = point3(500, 300, -1000);
                 camera.cameraLookAt = point3(280, 280, 0);
                 camera.cameraUp     = vec3(0,1,0);

                 
                 scene.add(make_shared<sphere>(point3( 123,700,-750),   300, sphericallight));
                     


                     int num_spheres = 500;
                     for (int j = 0; j < num_spheres; j++) {
                      shared_ptr<material> mat;

                      auto roughness = RandomDouble(0,0.3); 

                     auto randomColour = RandomDouble();
                     auto albedo = colour(0.6,0.6,0.5);
                     if(randomColour < 0.8){
                      albedo = colour(0.6,0.6,0.5);

                    
                     } else if(randomColour < 0.95){
                      albedo = colour(0.8,0.6,0.8);
                     } 
                     mat = make_shared<metal>(albedo, roughness);
                     scene.add(make_shared<sphere>(point3::random(0,500), 40, mat));
                     }

                    break;


                    
                    

   
                   
                
                  
            }
        }
        if (isSelected) {
            ImGui::SetItemDefaultFocus();
        }
    }
    ImGui::EndCombo();
}

 

    ImGui::Render();

    SDL_RenderClear(renderer);

    
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    SDL_RenderPresent(renderer);

    


    }
    


   

  
   



   
    

   
    

    

    ImGui_ImplSDLRenderer2_Shutdown();
   ImGui_ImplSDL2_Shutdown();   
   ImGui::DestroyContext(); 

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}