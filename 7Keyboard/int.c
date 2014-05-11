#include "kernel.h"
#define PORT_KEYDAT		0x0060



struct KEYBUF keybuf = {.data = "0", .next_r = 0, .next_w = 0};

void init_pic(void)
{
	_io_out8(PIC0_IMR,  0xff  ); 
	_io_out8(PIC1_IMR,  0xff  ); 

	_io_out8(PIC0_ICW1, 0x11  ); 
	_io_out8(PIC0_ICW2, 0x20  ); 
	_io_out8(PIC0_ICW3, 1 << 2); 
	_io_out8(PIC0_ICW4, 0x01  ); 

	_io_out8(PIC1_ICW1, 0x11  ); 
	_io_out8(PIC1_ICW2, 0x28  ); 
	_io_out8(PIC1_ICW3, 2     ); 
	_io_out8(PIC1_ICW4, 0x01  ); 

	_io_out8(PIC0_IMR,  0xfb  ); 
	_io_out8(PIC1_IMR,  0xff  ); 

	return;
}

void inthandler21(int *esp)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	unsigned char data, s[4];
	_io_out8(PIC0_OCW2, 0x61);
	data = _io_in8(PORT_KEYDAT);

	if ( (keybuf.next_w + 1) % KEYBUF_LEN < keybuf.next_r || keybuf.next_w >= keybuf.next_r ){
		keybuf.data[keybuf.next_w] = data;
		keybuf.next_w = (keybuf.next_w  + 1) % KEYBUF_LEN;
	}
//	char2hex(s, data, 4);	
//	boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
//	putfont8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
 	return ;
}

void inthandler2c(int *esp)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
	putfont8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");
	for (;;) {
		_io_hlt();
	}
}

void inthandler27(int *esp)
{
	_io_out8(PIC0_OCW2, 0x67); 
	return;
}
