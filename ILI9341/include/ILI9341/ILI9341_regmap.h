#ifndef ILI9341_REGMAP_H_
#define ILI9341_REGMAP_H_

#ifdef __cplusplus
    extern "C"
    {
#endif
	
	/*
	*	Note : Each read command reads 1 dummy byte,
	*	Driver8080 may ignores this byte.
	**/

	#define ILI9341_NOP		                0x00

	#define	ILI9341_SOFT_RST	            0x01
	///	3 data bytes
	#define	ILI9341_READ_ID	                0x04
	///	4 data bytes
	#define	ILI9341_READ_DISP_STATUS	    0x09
	///	1 data byte
	#define	ILI9341_READ_PWR_MODE	        0x0A
	///	1 data byte
	#define	ILI9341_READ_DISP_MADCTL	    0x0B
	///	1 data byte
	#define	ILI9341_READ_DISP_PIXEL_FMT	    0x0C
	///	1 data byte
	#define	ILI9341_READ_DISP_IMG_FMT	    0x0D
	///	1 data byte
	#define	ILI9341_READ_DISP_SNL_MOD	    0x0E
	///	1 data byte
	#define	ILI9341_READ_DISP_SLF_DIAGNSTC	0x0F

	#define	ILI9341_ENTER_SLEEP_MODE	    0x10
	#define	ILI9341_SLEEP_OUT	            0x11
	#define	ILI9341_PARTIAL_MODE	        0x12
	#define	ILI9341_NORMAL_MODE	            0x13
	#define	ILI9341_DISP_INV_OFF	        0x20
	#define	ILI9341_DISP_INV_ON	            0x21

	///	Send 1 data byte
	#define	ILI9341_RGMP_GAMMA_SET	        0x26

    #define ILI9341_DISPLAY_OFF             0x28
    #define ILI9341_DISPLAY_ON		        0x29
	
	///	Send 4 data bytes
	#define	ILI9341_COL_ADDR_SET	        0x2A
	///	Send 4 data bytes
	#define	ILI9341_PAG_ADDR_SET	        0x2B

	///	Send Graphic bytes
	#define	ILI9341_GRAM_WRITE		        0x2C

	///	128 bytes required
	#define	ILI9341_COLOR_SET		        0x2D

	///	Read Graphic bytes
	#define	ILI9341_GRAM_READ		        0x2E

	///	Send 4 bytes
	#define	ILI9341_PARTIAL_AREA	        0x30
	///	Send 6 bytes
	#define	ILI9341_V_SCROLL		        0x33

	#define	ILI9341_TEAR_OFF		        0x34
	#define ILI9341_TEAR_ON			        0x35

	///	Send 1 byte
	#define	ILI9341_MEM_ACCESS_CTL	        0x36

	///	Send 2 bytes
	#define	ILI9341_V_SCROLL_START	        0x37
	#define	ILI9341_IDLE_OFF		        0x38
	#define	ILI9341_IDLE_ON			        0x39

	///	Send 1 bytes
	#define	ILI9341_PIXEL_FMT_SET           0x3A

	///	Send Graphic bytes
	#define	ILI9341_WRITE_GRAM_CONT         0x3C

	///	Read Graphic bytes
	#define	ILI9341_READ_GRAM_CONT          0x3E

	///	Send 2 bytes
	#define	ILI9341_SET_TEAR_SCANLN         0x44

	///	Read 2 bytes
	#define	ILI9341_GET_SCANLN              0x45

	///	Send 1 byte
	#define	ILI9341_SET_BRIGTHNESS          0x51
	
	///	Read 1 byte
	#define	ILI9341_READ_BRIGTHNESS         0x52

	///	Send 1 byte
	#define	ILI9341_WR_CTL_DISP             0x53

	///	Read 1 byte
	#define	ILI9341_RD_CTL_DISP             0x54

	///	Send 1 byte
	#define	ILI9341_WR_ADAPTIVE_B_CTL	    0x55
	
	///	Read 1 byte
	#define	ILI9341_RD_ADAPTIVE_B_CTL	    0x56

	///	Send 1 byte
	#define	ILI9341_WR_ADAPTIVE_CABC_CTL	0x5E
	
	/// Read 1 byte
	#define	ILI9341_RD_ADAPTIVE_CABC_CTL	0x5F

	/// Read 1 byte
	#define	ILI9341_READ_ID1			    0xDA
	/// Read 1 byte
	#define	ILI9341_READ_ID2			    0xDB
	/// Read 1 byte
	#define	ILI9341_READ_ID3			    0xDC

	///
	///	Extended command set
	///

	///	Send 1 byte
	#define	ILI9341_RGB_INTERFACE_CTL	    0xB0
	///	Send 1 byte
	#define	ILI9341_FRAME_CTL_NORMAL	    0xB1
	///	Send 1 byte
	#define	ILI9341_FRAME_CTL_IDLE		    0xB2
	///	Send 1 byte
	#define	ILI9341_DISP_INV_CTL		    0xB4
	///	Send 4 bytes
	#define	ILI9341_BLAK_PORCH_CTL		    0xB5

	///	Send 4 bytes
	#define ILI9341_DISP_FCN_CTL		    0xB6
	///	Send 1 byte
	#define ILI9341_ENTRY_MODE_SET		    0xB7

	///	Send 1 byte
	#define ILI9341_BACKLIGTH_CTL1		    0xB8
	///	Send 1 byte
	#define ILI9341_BACKLIGTH_CTL2		    0xB9
	///	Send 1 byte
	#define ILI9341_BACKLIGTH_CTL3		    0xBA
	///	Send 1 byte
	#define ILI9341_BACKLIGTH_CTL4		    0xBB
	///	Send 1 byte
	#define ILI9341_BACKLIGTH_CTL5		    0xBC
	///	Send 1 byte
	#define ILI9341_BACKLIGTH_CTL7		    0xBE
	///	Send 1 byte
	#define ILI9341_BACKLIGTH_CTL8		    0xBF


	///	Send 1 byte
	#define ILI9341_POWER_CTL1			    0xC0
	///	Send 1 byte
	#define ILI9341_POWER_CTL2			    0xC1
	///	Send 2 byte
	#define ILI9341_VCOM_CTL1			    0xC5
	///	Send 1 byte
	#define ILI9341_VCOM_CTL2			    0xC7
	///	Send 2 byte
	#define ILI9341_NV_MEM_WRITE		    0xD0
	///	Send 3 byte
	#define ILI9341_NV_MEM_PROTECT_KEY	    0xD1
	///	Read 2 byte
	#define ILI9341_NV_MEM_STATUS_READ	    0xD2
	
	///	Read 3 bytes
	#define ILI9341_READ_ID4			    0xD3
	///	Send 15 bytes
	#define ILI9341_POS_GAMMA_CORREC	    0xE0
	///	Send 15 bytes
	#define ILI9341_NEG_GAMMA_CORREC	    0xE1
	///	Send 64 bytes
	#define ILI9341_DIG_GAMMA_CTL1		    0xE2
	
	///	Send 3 bytes
	#define ILI9341_INTERFACE_CTL		    0xF6
 
#ifdef __cplusplus
    }
#endif

#endif
