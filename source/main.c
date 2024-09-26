#include <3ds.h>
#include <citro2d.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

enum Zone {
	RED,
	WHITE,
	GREEN,
	BLUE
};

enum Zone updateZone(u16 tx, u16 ty) {
	enum Zone current;
	// RED
	if ((tx >= 0 && tx <= SCREEN_WIDTH / 2) && (ty >= 0 && ty <= SCREEN_HEIGHT / 2)) {
		current = RED;
	}

	// WHITE
	if ((tx >= 160 && tx <= 320) && (ty >= 0 && ty <= SCREEN_HEIGHT / 2)) {
		current = WHITE;
	}

	// GREEN
	if ((tx >= 0 && tx <= SCREEN_WIDTH / 2) && (ty >= SCREEN_HEIGHT / 2 && ty <= SCREEN_HEIGHT)) {
		current = GREEN;
	}

	// BLUE
	if ((tx >= SCREEN_WIDTH / 2 && tx <= SCREEN_WIDTH) && (ty >= SCREEN_HEIGHT / 2 && ty <= SCREEN_HEIGHT)) {
		current = BLUE;
	}

	return current;
}

int main(int argc, char **argv)
{
	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_TOP, NULL);

	u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
	u32 clrGreen = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
	u32 clrRed   = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
	u32 clrBlue  = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);
	u32 clrClear = C2D_Color32(0x00, 0x00, 0x00, 0x00);

	C3D_RenderTarget* bot = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	enum Zone currentZone = RED;

	// Main loop
	while (aptMainLoop())
	{
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break; // break in order to return to hbmenu

		touchPosition touch;

		//Read the touch screen coordinates
		hidTouchRead(&touch);

		// update current zone if input detected (not 0,0)
		if (touch.px != 0 || touch.py != 0) {
			currentZone = updateZone(touch.px, touch.py);
		}

		//Print the touch screen coordinates
		printf("\x1b[1;0HTouch Screen position:");
		printf("\x1b[2;0H%03d; %03d", touch.px, touch.py);

		// Print the current zone
		if (currentZone == RED) {
			printf("\x1b[3;0HCurrent Zone: RED  ");
		} else if (currentZone == WHITE) {
			printf("\x1b[3;0HCurrent Zone: WHITE");
		} else if (currentZone == GREEN) {
			printf("\x1b[3;0HCurrent Zone: GREEN");
		} else {
			printf("\x1b[3;0HCurrent Zone: BLUE ");
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(bot, clrClear);
		C2D_SceneBegin(bot);

		C2D_DrawRectSolid(0, 0, 0, 160, 120, clrRed);
		C2D_DrawRectSolid(0, SCREEN_HEIGHT - 120, 0, 160, 120, clrGreen);

		C2D_DrawRectSolid(SCREEN_WIDTH - 160, 0, 0, 160, 120, clrWhite);
		C2D_DrawRectSolid(SCREEN_WIDTH - 160, SCREEN_HEIGHT - 120, 0, 160, 120, clrBlue);

		C3D_FrameEnd(0);
	}

	// Exit services
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}
