//DT Nesimi

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> ///include the header to initialize the image addon
#include <allegro5/allegro_native_dialog.h>
#include <iostream>
# define M_PI           3.14159265358979323846  /* pi */

int main(int argc, char** argv) {

	const float FPS = 60;
	const int SCREEN_W = 640;
	const int SCREEN_H = 480;
	const int sprite_SIZE = 64;
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_EVENT Event;

	float sprite_x = SCREEN_W / 2.0 - sprite_SIZE / 2.0;
	float sprite_y = SCREEN_H / 2.0 - sprite_SIZE / 2.0;

	float sprite_dx = 0, sprite_dy = 0; //reset to 0 for starting

	float angle = 0; // added angle
	bool turning = false; // added rotation check

	bool redraw = true;
	ALLEGRO_BITMAP* image = NULL;
	ALLEGRO_BITMAP* sprite = NULL;

	if (!al_init()) {
		return -1;
	}

	if (!al_install_keyboard()) {
		std::cerr << "Failed to initialize the keyboard!\n";
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display) {
		al_destroy_timer(timer);
		return -1;
	}
	if (!al_install_keyboard()) {
		std::cerr << "Failed to initialize the keyboard!\n";
		return -1;
	}



	al_init_image_addon();
	image = al_load_bitmap("pond.png");
	sprite = al_load_bitmap("forg.png");
	event_queue = al_create_event_queue();
	if (!event_queue) {
		al_destroy_bitmap(sprite);
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	while (1)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);


		if (ev.type == ALLEGRO_EVENT_TIMER) {
			
			if (!turning) {

				sprite_x += sprite_dx;
				sprite_y += sprite_dy;

				if (sprite_x < 0 || sprite_x > SCREEN_W - sprite_SIZE) {
					sprite_dx = 0;
					turning = true;
				}
				else if (sprite_y < 0 || sprite_y > SCREEN_H - sprite_SIZE) {
					sprite_dy = 0;
					turning = true;
				}
			}
			else {

				if (sprite_x < 0) {
					angle += 0.1;
					if (angle >= M_PI/2) {
						sprite_dx = 4;
						turning = false;
					}
				}
				else if (sprite_x > SCREEN_W - sprite_SIZE) {
					angle -= 0.1;
					if (angle <= -M_PI/2) {
						sprite_dx = -4;
						turning = false;
					}
				} else if (sprite_y < 0) {
					angle += 0.1;
					if (angle >= M_PI) {
						sprite_dy = 4;
						turning = false;
					}
				}
				else if (sprite_y > SCREEN_H - sprite_SIZE) {
					angle -= 0.1;
					if (angle <= 0) {
						sprite_dy = -4;
						turning = false;
					}
				}
			}

			redraw = true;

		} else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_LEFT:
				sprite_dx = -4;
				sprite_dy = 0;
				turning = false;
				angle = -M_PI/2;
				break;
			case ALLEGRO_KEY_RIGHT:
				sprite_dx = 4;
				sprite_dy = 0;
				turning = false;
				angle = M_PI/2;
				break;
			case ALLEGRO_KEY_UP:
				sprite_dy = -4;
				sprite_dx = 0;
				turning = false;
				angle = 0;
				break;
			case ALLEGRO_KEY_DOWN:
				sprite_dy = 4;
				sprite_dx = 0;
				turning = false;
				angle = -M_PI;
				break;
			}
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(image, 0, 0, 0);
			al_draw_rotated_bitmap(sprite, 32, 32, sprite_x + 32, sprite_y + 32, angle, 0);
			al_flip_display();
		}
	}

	al_destroy_bitmap(sprite);
	al_destroy_bitmap(image);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	system("pause");
	return 0;
}