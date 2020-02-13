// switch_windows.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
//#include <future>
#include <list>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <vector>
#undef main

//const int DELAY = 5000;

void text(SDL_Renderer *ren, int x, int y, const char *ispis, TTF_Font *font, SDL_Rect *rect, SDL_Color *color, int *wi, int he, int *ip);
void draw_checkbox(SDL_Renderer *ren, int x, int y, int wi, int he);
void draw_checked_checkbox(SDL_Renderer *ren, int x, int y, int wi, int he);

std::list<std::string> windows_names;
std::list <std::string> split_string(std::string user_input);
std::list <HWND> list_of_HWNDs;
void show_windows(std::list<std::string> user_input, std::list <HWND> list_of_HWNDs);

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	char WinText[256] = { NULL };

	if (!hwnd || !IsWindowVisible(hwnd)) // if hwnd == NULL || if the window is not visible
		return 1; // continue recursion
	else {
		GetWindowTextA(hwnd, WinText, sizeof(WinText));
		if (strlen(WinText) != 0) {
			windows_names.push_back(WinText);
			list_of_HWNDs.push_back(hwnd);
		}
		return 1; // continue recursion
	}
}
int main()
{
	int wi = 0, ip = 0,x,y;
	SDL_Window *win = SDL_CreateWindow("SWITCH_WINDOWS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1330, 685, SDL_WINDOW_OPENGL);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Rect rect;
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 36), *font2 = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 18);
	SDL_Color color = { 255, 255, 0, 255 }, color2 = { 255,0,0,255 }, color3 = { 0,255,0,255 }, color4 = { 0,0,0,255 };
	SDL_Event ev;
	
	//std::string user_input;
	int width = 20, height = 20, text_width = 1200;
	int counter = 0; 
	std::list<std::string> chosen_windows;  //negde dalje u kodu se koristi, nebitno
	std::vector<int> checked_windows;
	std::vector<const char*> windows_names_const;
	
	EnumWindows(EnumWindowsProc, 0);	//popunjava windows_names

    for (std::list<std::string>::const_iterator i = windows_names.cbegin(); i != windows_names.cend(); ++i) {
        
		if (!i->length()){
            continue;
		}

		draw_checkbox(ren, 0, height * counter, width,height);
        windows_names_const.emplace_back(i->c_str());
        text(ren, 50, height * counter, windows_names_const[counter],font2, &rect, &color2, &text_width, height, &ip);
		counter ++;
		checked_windows.push_back(0);
    }

	text(ren, 1000, 550, "Apply", font2, &rect, &color2, &text_width, height, &ip);
	SDL_RenderPresent(ren);
	
	while (1) {
		SDL_PollEvent(&ev);
		SDL_GetMouseState(&x, &y);
		if (x > 1000 && x < 1330 && y>550 && y < 586) {
			text(ren, 1000, 550, "Apply", font2, &rect, &color3, &wi, height, &ip);
			SDL_RenderPresent(ren);
			if (ev.type == SDL_MOUSEBUTTONDOWN)
				break;
		}

		else{
			text(ren, 1000, 550, "Apply", font2, &rect, &color2, &text_width, height, &ip);
			SDL_RenderPresent(ren);
		}
		
		if (y < ip) {
			
			for (int i = 0; i < counter; i++) {
				
				if (y > i*height && y < i*height + 20) {
					std::cout<<windows_names_const[i]<<std::endl;
					text(ren, 50, height * i, windows_names_const[i], font2, &rect, &color3, &text_width, height, &ip);
					SDL_RenderPresent(ren);
					
					if (ev.type == SDL_MOUSEBUTTONDOWN) {

						if (checked_windows[i] == 0) {
							checked_windows[i] = 1;
							draw_checked_checkbox(ren, 0, i * height, width, height);
							SDL_RenderPresent(ren);
							Sleep(200);
						}

						else {
							checked_windows[i] = 0;
							draw_checkbox(ren, 0, i*height, width, height);
							SDL_RenderPresent(ren);
							Sleep(200);	
						}
					}
				}

				else {
					text(ren, 50, height * i, windows_names_const[i], font2, &rect, &color2, &text_width, height, &ip);
					SDL_RenderPresent(ren);
				}
			}
		}

		else {
			for (int i = 0; i < counter; i++) {
				text(ren, 50, height * i, windows_names_const[i], font2, &rect, &color2, &text_width, height, &ip);
				SDL_RenderPresent(ren);
			}
		}
	}

	int value = 0;

	for each( int i in checked_windows){
			
		if (i == 1){
			chosen_windows.push_back (std::to_string(static_cast<long long>(value + 1)));
		 }
		value++;
	}

	while (1) {
			text(ren, 1000, 650, "Stop", font2, &rect, &color2, &text_width, height, &ip);
			SDL_RenderPresent(ren);
			show_windows(chosen_windows, list_of_HWNDs);
	}

	return 0;
}

std::list <std::string> split_string(std::string user_input) {
	std::list <std::string> result;
	std::string temp = "";
	int i = 0;
	while (user_input[i] != '\0') {
		if (user_input[i] != ',') {
			temp += user_input[i];
		}
		else {
			result.push_back(temp);
			temp = "";
		}
		i++;
	}
	result.push_back(temp);
	return result;
}


void show_windows(std::list<std::string> user_input, std::list <HWND> list_of_HWNDs) {
	int counter_int;
	for each (std::string counter in user_input){
//	for (auto const& counter : user_input) {
		counter_int = stoi(counter);
		std::list<HWND>::iterator iterator = list_of_HWNDs.begin();
		std::advance(iterator, counter_int - 1);
		SetForegroundWindow(*iterator);
		Sleep(5000);
	}
}



void text(SDL_Renderer *ren,int x,int y,const char *ispis,TTF_Font *font,SDL_Rect *rect,SDL_Color *color,int *wi,int he,int *ip){
	SDL_Texture *texture;
	SDL_Surface *surface  = TTF_RenderUTF8_Blended(font, ispis, *color);
//surface= TTF_RenderText_Blended(font, ispis, *color);
	if (!(surface= TTF_RenderText_Blended(font, ispis, *color))){
		std::cout<<TTF_GetError();
	}
	texture = SDL_CreateTextureFromSurface(ren, surface);
	rect->x=x;
	rect->y=y;
	*wi=surface->w;
	surface->h=he;
	*ip=y+he;
	rect->w=surface->w;
	rect->h=surface->h;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(ren,texture,NULL,rect);
	SDL_DestroyTexture(texture);
}


void draw_checkbox(SDL_Renderer *ren, int x, int y,int wi, int he) {
	SDL_Surface *image;
	SDL_Texture *texture;
	image = SDL_LoadBMP("D:/switch_windows/switch_windows/switch_windows/unchecked.bmp");

	texture = SDL_CreateTextureFromSurface(ren, image);
	SDL_RenderPresent(ren);
	

	SDL_Rect dstrect = { x, y, wi, he};
	SDL_RenderCopy(ren, texture, NULL, &dstrect);
	texture = SDL_CreateTextureFromSurface(ren, image);
	SDL_RenderPresent(ren);
	
	
}

void draw_checked_checkbox(SDL_Renderer *ren, int x, int y, int wi, int he) {
	SDL_Surface *image;
	SDL_Texture *texture;
	image = SDL_LoadBMP("D:/switch_windows/switch_windows/switch_windows/checked.bmp");

	texture = SDL_CreateTextureFromSurface(ren, image);
	SDL_RenderPresent(ren);


	SDL_Rect dstrect = { x, y, wi, he };
	SDL_RenderCopy(ren, texture, NULL, &dstrect);
	texture = SDL_CreateTextureFromSurface(ren, image);
	SDL_RenderPresent(ren);
}

/*text(ren, 1000, 650, "Apply", font2, &rect, &color, &wi, 36, &ip);
SDL_RenderPresent(ren);


SDL_PollEvent(&ev);
SDL_GetMouseState(&x, &y);
if (x > 1000 && x < 1330 && y>650 && y < 686) {
text(ren, 1000, 650, "Apply", font2, &rect, &color2, &wi, 36, &ip);
if (ev.type == SDL_MOUSEBUTTONDOWN)
text(ren, 1000, 650, "Apply", font2, &rect, &color3, &wi, 36, &ip);
}
*/
