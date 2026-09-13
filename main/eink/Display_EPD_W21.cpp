#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"

//Delay Functions
void delay_xms(unsigned int xms)
{
  DEV_Delay_ms(xms);
}

////////////////////////////////////E-paper demo//////////////////////////////////////////////////////////
//Busy function
void Epaper_READBUSY(void)
{ 
  while(1)
  {	 //=1 BUSY
     if(isEPD_W21_BUSY==0) break;
     DEV_Delay_ms(10);
  }  
}

//Full screen update initialization
void EPD_HW_Init(void)
{
	EPD_W21_RST_0;  // Module reset   
	delay_xms(10);//At least 10ms delay 
	EPD_W21_RST_1;
	delay_xms(10); //At least 10ms delay 
	
	Epaper_READBUSY();   
	EPD_W21_WriteCMD(0x12);  //SWRESET
	Epaper_READBUSY();   
		
	EPD_W21_WriteCMD(0x01); //Driver output control      
	EPD_W21_WriteDATA((EPD_HEIGHT-1)%256);    
	EPD_W21_WriteDATA((EPD_HEIGHT-1)/256);
	EPD_W21_WriteDATA(0x00);

	EPD_W21_WriteCMD(0x11); //data entry mode       
	EPD_W21_WriteDATA(0x03);

	EPD_W21_WriteCMD(0x44); //set Ram-X address start/end position   
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(EPD_WIDTH/8-1);   

	EPD_W21_WriteCMD(0x45); //set Ram-Y address start/end position
    EPD_W21_WriteDATA(0x00);
    EPD_W21_WriteDATA(0x00);
    EPD_W21_WriteDATA((EPD_HEIGHT-1)%256);
    EPD_W21_WriteDATA((EPD_HEIGHT-1)/256);

	EPD_W21_WriteCMD(0x3C); //BorderWavefrom
	EPD_W21_WriteDATA(0x05);	

	EPD_W21_WriteCMD(0x21); //  Display update control
	EPD_W21_WriteDATA(0x00);		
  EPD_W21_WriteDATA(0x80);	
	
  EPD_W21_WriteCMD(0x18); //Read built-in temperature sensor
	EPD_W21_WriteDATA(0x80);	

	EPD_W21_WriteCMD(0x4E);   // set RAM x address count to 0;
	EPD_W21_WriteDATA(0x00);
    EPD_W21_WriteCMD(0x4F);   // set RAM y address count
    EPD_W21_WriteDATA(0x00);
    EPD_W21_WriteDATA(0x00);
  Epaper_READBUSY();
	
}
//Fast full screen update initialization
void EPD_HW_Init_Fast(void)		// 1.6s 
{
	EPD_W21_RST_0;  // Module reset   
	delay_xms(10);//At least 10ms delay 
	EPD_W21_RST_1;
	delay_xms(10); //At least 10ms delay   
	
	EPD_W21_WriteCMD(0x12);  //SWRESET
	Epaper_READBUSY();   
 	
  EPD_W21_WriteCMD(0x18); //Read built-in temperature sensor
	EPD_W21_WriteDATA(0x80);	
	  	
	EPD_W21_WriteCMD(0x22); // Load temperature value
	EPD_W21_WriteDATA(0xB1);		
  EPD_W21_WriteCMD(0x20);	
  Epaper_READBUSY();   

	EPD_W21_WriteCMD(0x1A); // Write to temperature register
	EPD_W21_WriteDATA(0x5a);		// 90		
  EPD_W21_WriteDATA(0x00);	
				  	
	EPD_W21_WriteCMD(0x22); // Load temperature value
	EPD_W21_WriteDATA(0x91);		
  EPD_W21_WriteCMD(0x20);	
	Epaper_READBUSY();   

    EPD_W21_WriteCMD(0x01); //Driver output control
    EPD_W21_WriteDATA((EPD_HEIGHT-1)%256);
    EPD_W21_WriteDATA((EPD_HEIGHT-1)/256);
    EPD_W21_WriteDATA(0x00);

    EPD_W21_WriteCMD(0x11); //data entry mode
    EPD_W21_WriteDATA(0x03);

    EPD_W21_WriteCMD(0x44); //set Ram-X address start/end position
    EPD_W21_WriteDATA(0x00);
    EPD_W21_WriteDATA(EPD_WIDTH/8-1);

    EPD_W21_WriteCMD(0x45); //set Ram-Y address start/end position
    EPD_W21_WriteDATA(0x00);
    EPD_W21_WriteDATA(0x00);
    EPD_W21_WriteDATA((EPD_HEIGHT-1)%256);
    EPD_W21_WriteDATA((EPD_HEIGHT-1)/256);

    EPD_W21_WriteCMD(0x4E); //set RAM x address count
    EPD_W21_WriteDATA(0x00);

    EPD_W21_WriteCMD(0x4F); //set RAM y address count
    EPD_W21_WriteDATA(0x00);
    EPD_W21_WriteDATA(0x00);

    Epaper_READBUSY();
}
//////////////////////////////Display Update Function///////////////////////////////////////////////////////
//Full screen update function
void EPD_Update(void)
{   
  EPD_W21_WriteCMD(0x22); //Display Update Control
  EPD_W21_WriteDATA(0xF7);   
  EPD_W21_WriteCMD(0x20); //Activate Display Update Sequence
  Epaper_READBUSY();   

}
//Fast full screen  update function
void EPD_Update_Fast(void)
{   
  EPD_W21_WriteCMD(0x22); //Display Update Control 
	EPD_W21_WriteDATA(0xC7);   
  EPD_W21_WriteCMD(0x20); //Activate Display Update Sequence
  Epaper_READBUSY();   

}
// Full screen  update function (Basemap)
void EPD_Update_BaseMap(void)
{   
  EPD_W21_WriteCMD(0x22); //Display Update Control
  EPD_W21_WriteDATA(0xF4);   
  EPD_W21_WriteCMD(0x20); //Activate Display Update Sequence
  Epaper_READBUSY();   

}
//Partial update function
void EPD_Part_Update(void)
{
	EPD_W21_WriteCMD(0x22); //Display Update Control
	EPD_W21_WriteDATA(0x1C);   
	EPD_W21_WriteCMD(0x20); //Activate Display Update Sequence
	Epaper_READBUSY(); 			
}
//////////////////////////////Display Data Transfer Function////////////////////////////////////////////
//Full screen update display function
void EPD_WhiteScreen_ALL(const unsigned char* datasBW,const unsigned char* datasRW)
{
    unsigned int i;
	  //Write Data
		EPD_W21_WriteCMD(0x24);	       //Transfer old data
	  for(i=0;i<EPD_ARRAY;i++)	  
    {	
	    EPD_W21_WriteDATA(datasBW[i]);  //Transfer the actual displayed data
    }	
		EPD_W21_WriteCMD(0x26);		     //Transfer new data
	  for(i=0;i<EPD_ARRAY;i++)	     
	  {
	    EPD_W21_WriteDATA(~datasRW[i]);  //Transfer the actual displayed data
	  }
    EPD_Update();	    

}
//Fast full screen update display function
void EPD_WhiteScreen_ALL_Fast(const unsigned char* datasBW,const unsigned char* datasRW)
{
    unsigned int i;
	  //Write Data
		EPD_W21_WriteCMD(0x24);	       //Transfer old data
	  for(i=0;i<EPD_ARRAY;i++)	  
    {	
	    EPD_W21_WriteDATA(datasBW[i]);  //Transfer the actual displayed data
    }	
		EPD_W21_WriteCMD(0x26);		     //Transfer new data
	  for(i=0;i<EPD_ARRAY;i++)	     
	  {
	    EPD_W21_WriteDATA(~datasRW[i]);  //Transfer the actual displayed data
	  }
    EPD_Update_Fast();	    

}
//Clear screen display
void EPD_WhiteScreen_White(void)
{
    unsigned int i;
	  //Write Data
		EPD_W21_WriteCMD(0x24);	       //Transfer old data
	  for(i=0;i<EPD_ARRAY;i++)	  
    {	
	    EPD_W21_WriteDATA(0xFF); 
    }
		EPD_W21_WriteCMD(0x26);		     //Transfer new data
	  for(i=0;i<EPD_ARRAY;i++)	     
	  {
	    EPD_W21_WriteDATA(0x00);  //Transfer the actual displayed data
	  }
    EPD_Update();	
	
}
//Clear screen display
void EPD_WhiteScreen_White_Fast(void)
{
    unsigned int i;
	
		EPD_W21_WriteCMD(0x3c);  // set border
		EPD_W21_WriteDATA(0x01);     // 0x00=black £» 0x01=white £» 0x80=VCOM £» 0xc0=HiZ£¨default£©	 	
	  //Write Data
		EPD_W21_WriteCMD(0x24);	       //Transfer old data
	  for(i=0;i<EPD_ARRAY;i++)	  
    {	
	    EPD_W21_WriteDATA(0xFF); 
    }
		EPD_W21_WriteCMD(0x26);		     //Transfer new data
	  for(i=0;i<EPD_ARRAY;i++)	     
	  {
	    EPD_W21_WriteDATA(0x00);  //Transfer the actual displayed data
	  }
    EPD_Update_Fast();	

		EPD_W21_WriteCMD(0x26);		     //Transfer new data
	  for(i=0;i<EPD_ARRAY;i++)	     
	  {
	    EPD_W21_WriteDATA(0xFF);  //Transfer the actual displayed data
	  }		
}
//Display all black
void EPD_WhiteScreen_Black(void)
{
	unsigned int i;
	//Write Data
	EPD_W21_WriteCMD(0x24);	       //Transfer old data
	for(i=0;i<EPD_ARRAY;i++)	  
	{	
		EPD_W21_WriteDATA(0xFF); 
	}
	EPD_W21_WriteCMD(0x26);		     //Transfer new data
	for(i=0;i<EPD_ARRAY;i++)	     
	{
		EPD_W21_WriteDATA(0xFF);  //Transfer the actual displayed data
	}
	EPD_Update();	
}

//Partial update of background display, this function is necessary, please do not delete it!!!
void EPD_SetRAMValue_BaseMap(const unsigned char* datasBW,const unsigned char* datasRW)
{
    unsigned int i;
	  unsigned char a;
	  //Write Data
		EPD_W21_WriteCMD(0x24);	       //Transfer old data
	  for(i=0;i<EPD_ARRAY;i++)	  
    {	
	    EPD_W21_WriteDATA(datasBW[i]);  //Transfer the actual displayed data
    }	
		EPD_W21_WriteCMD(0x26);		     //Transfer new data
	  for(i=0;i<EPD_ARRAY;i++)	     
	  {
	    EPD_W21_WriteDATA(~datasRW[i]);  //Transfer the actual displayed data
	  }
    EPD_Update_BaseMap();	  	 
	  Epaper_READBUSY();   
		
	  //update
		EPD_W21_WriteCMD(0x24);	       //Transfer old data
	  for(i=0;i<EPD_ARRAY;i++)	  
    {	
			a=datasBW[i]&datasRW[i];
	    EPD_W21_WriteDATA(a);  //Transfer the actual displayed data
    }	
		EPD_W21_WriteCMD(0x26);		     //Transfer new data
	  for(i=0;i<EPD_ARRAY;i++)	  
    {	
			a=datasBW[i]&datasRW[i];
	    EPD_W21_WriteDATA(a);  //Transfer the actual displayed data
    }	
		
		//////////////////////////////////////////////
}
//Partial update display
void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE)
{
	unsigned int i;  
	unsigned int x_end,y_end;
	
	x_start=x_start/8; //x address start
	x_end=x_start+PART_LINE/8-1; //x address end
	//y_start=y_start; //Y address start
	y_end=y_start+PART_COLUMN-1; //Y address end

	EPD_W21_WriteCMD(0x44);       // set RAM x address start/end
	EPD_W21_WriteDATA(x_start);  //x address start
	EPD_W21_WriteDATA(x_end);   //y address end   
	EPD_W21_WriteCMD(0x45);    // set RAM y address start/end
	EPD_W21_WriteDATA(y_start%256);  //y address start2 
	EPD_W21_WriteDATA(y_start/256); //y address start1 
	EPD_W21_WriteDATA(y_end%256);  //y address end2 
	EPD_W21_WriteDATA(y_end/256); //y address end1   

	EPD_W21_WriteCMD(0x4E);        // set RAM x address count to 0;
	EPD_W21_WriteDATA(x_start);   //x start address
	EPD_W21_WriteCMD(0x4F);      // set RAM y address count to 0X127;    
	EPD_W21_WriteDATA(y_start%256);//y address start2
	EPD_W21_WriteDATA(y_start/256);//y address start1
	
	
	 EPD_W21_WriteCMD(0x24);   //Write Black and White image to RAM
   for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     EPD_W21_WriteDATA(datas[i]);
   } 
	 EPD_Part_Update();

}



//Experimental fast black/white refresh for the 296x128 SSD1680A B/W/R panel family.
//This LUT and the 227-byte expansion are ported directly from the open-source
//cc2640r2-etag driver's 2.9-inch SSD1680A fast clock refresh implementation.
//Unlike SSD1680 Mode-2 differential refresh, this uses the B/W/R Mode-1 LUT
//and writes the desired B/W image directly, with the red RAM held clear.
#define EPD_FAST_BW_VSS   0x00
#define EPD_FAST_BW_VSH1  0x40
#define EPD_FAST_BW_VSL   0x80
#define EPD_FAST_BW_VSH2  0xC0

static const unsigned char EPD_FAST_BW_LUT_LITE[35] =
{
    // RP,   A,                         B,    C,                         D,    SRAB, SRCD
    0x00,   0x00,                      0x00, 0x00,                      0x00, 0x00, 0x00, // LUTC

    // Hold red pixels in place during fast B/W refresh. Red pixels select
    // LUTR in SSD1680A 3-color mode, so skipping LUTR means those pixels
    // receive no source-drive phases during this update.
    0x00,   0x00,                      0x00,
            0x00,                      0x00, 0x00, 0x00,               // LUTR

    0x01,   EPD_FAST_BW_VSL  | 0x3F,  0x00,
            0x00,                      0x00, 0x02, 0x00,               // LUTW

    // Stronger single-pass black drive.
    // 0x3F ~= two 0x1F passes worth of phase length.
    0x01,   EPD_FAST_BW_VSH1 | 0x3F,  0x00,
            0x00,                      0x00, 0x01, 0x00,               // LUTB

    // FR: 5 = 125 Hz
    0x05,

    // EOPT, VGH, VSH1, VSH2, VSL, VCOM
    0x22, 0x17, 0x41, 0x94, 0x32, 0x36
};

static void EPD_Load_Fast_BW_LUT_A(void)
{
    // SSD1680A uses a 227-byte waveform payload for command 0x32.
    // The "lite" LUT stores only the first 7-byte group for LUTC/R/W/B;
    // the remaining seven groups of each LUT are zero-filled.
    EPD_W21_WriteCMD(0x32);

    for (unsigned int lut = 0; lut < 4; lut++)
    {
        for (unsigned int i = 0; i < 7; i++)
        {
            EPD_W21_WriteDATA(EPD_FAST_BW_LUT_LITE[lut * 7 + i]);
        }

        for (unsigned int i = 0; i < 49; i++)
        {
            EPD_W21_WriteDATA(0x00);
        }
    }

    // 4 * (7 + 49) = 224 bytes, then FR + 2 XON bytes = 227.
    EPD_W21_WriteDATA(EPD_FAST_BW_LUT_LITE[28]);
    EPD_W21_WriteDATA(0x00);
    EPD_W21_WriteDATA(0x00);

    EPD_W21_WriteCMD(0x3F); // LUT end option
    EPD_W21_WriteDATA(EPD_FAST_BW_LUT_LITE[29]);

    EPD_W21_WriteCMD(0x03); // gate voltage
    EPD_W21_WriteDATA(EPD_FAST_BW_LUT_LITE[30]);

    EPD_W21_WriteCMD(0x04); // source voltages
    EPD_W21_WriteDATA(EPD_FAST_BW_LUT_LITE[31]); // VSH1
    EPD_W21_WriteDATA(EPD_FAST_BW_LUT_LITE[32]); // VSH2
    EPD_W21_WriteDATA(EPD_FAST_BW_LUT_LITE[33]); // VSL

    EPD_W21_WriteCMD(0x2C); // VCOM
    EPD_W21_WriteDATA(EPD_FAST_BW_LUT_LITE[34]);
}

void EPD_HW_Init_Fast_BW_A(void)
{
    // Keep the exact geometry/orientation initialization that is already
    // proven on this panel, then override only the waveform/voltages.
    EPD_HW_Init();
    EPD_Load_Fast_BW_LUT_A();
}

void EPD_WhiteScreen_BW_Fast_A(const unsigned char* datasBW, const unsigned char* datasRW)
{
    // Desired black/white image. Canvas uses 0 = ink. Red pixels are forced
    // to 0 in B/W RAM, giving the SSD1680A's canonical red encoding:
    // R RAM = 1, B/W RAM = 0.
    EPD_W21_WriteCMD(0x24);
    for (unsigned int i = 0; i < EPD_ARRAY; i++)
    {
        EPD_W21_WriteDATA(datasBW[i] & datasRW[i]);
    }

    // Preserve the current red canvas instead of clearing it. Canvas uses
    // 0 = red ink, while SSD1680A red RAM uses 1 = red.
    EPD_W21_WriteCMD(0x26);
    for (unsigned int i = 0; i < EPD_ARRAY; i++)
    {
        EPD_W21_WriteDATA((unsigned char)~datasRW[i]);
    }

    // Display Mode 1 using the custom fast waveform. LUTR is intentionally
    // disabled above, so pixels marked red are held rather than refreshed.
    EPD_W21_WriteCMD(0x22);
    EPD_W21_WriteDATA(0xC7);
    EPD_W21_WriteCMD(0x20);
    Epaper_READBUSY();
}


//Deep sleep function
void EPD_DeepSleep(void)
{  	
  EPD_W21_WriteCMD(0x10); //Enter deep sleep
  EPD_W21_WriteDATA(0x01); 
  delay_xms(100);
}

//Partial update write address and data
void EPD_Dis_Part_RAM(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE)
{
	unsigned int i;  
	unsigned int x_end,y_end;
	
	x_start=x_start/8; //x address start
	x_end=x_start+PART_LINE/8-1; //x address end
	
	y_start=y_start-1; //Y address start
	y_end=y_start+PART_COLUMN-1; //Y address end
	
	EPD_W21_WriteCMD(0x44);       // set RAM x address start/end
	EPD_W21_WriteDATA(x_start);  //x address start
	EPD_W21_WriteDATA(x_end);   //y address end   
	EPD_W21_WriteCMD(0x45);     // set RAM y address start/end
	EPD_W21_WriteDATA(y_start%256);  //y address start2 
	EPD_W21_WriteDATA(y_start/256); //y address start1 
	EPD_W21_WriteDATA(y_end%256);  //y address end2 
	EPD_W21_WriteDATA(y_end/256); //y address end1   

	EPD_W21_WriteCMD(0x4E);   // set RAM x address count to 0;
	EPD_W21_WriteDATA(x_start);   //x start address
	EPD_W21_WriteCMD(0x4F);   // set RAM y address count to 0X127;    
	EPD_W21_WriteDATA(y_start%256); //y address start2
	EPD_W21_WriteDATA(y_start/256); //y address start1
		
	EPD_W21_WriteCMD(0x24);   //Write Black and White image to RAM
  for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
   {                         
     EPD_W21_WriteDATA(datas[i]);
   } 
}
//Num display
void EPD_Dis_Part_Num(unsigned int x_startA,unsigned int y_startA,const unsigned char * datasA,
	                       unsigned int x_startB,unsigned int y_startB,const unsigned char * datasB,
												 unsigned int x_startC,unsigned int y_startC,const unsigned char * datasC,
											   unsigned int x_startD,unsigned int y_startD,const unsigned char * datasD,
												 unsigned int PART_COLUMN,unsigned int PART_LINE
	                      )
{
	EPD_Dis_Part_RAM(x_startA,y_startA,datasA,PART_COLUMN,PART_LINE);
	EPD_Dis_Part_RAM(x_startB,y_startB,datasB,PART_COLUMN,PART_LINE);
	EPD_Dis_Part_RAM(x_startC,y_startC,datasC,PART_COLUMN,PART_LINE);
	EPD_Dis_Part_RAM(x_startD,y_startD,datasD,PART_COLUMN,PART_LINE);
	EPD_Part_Update();
}												 




////////////////////////////////Other newly added functions////////////////////////////////////////////
//Display rotation 180 degrees initialization
void EPD_HW_Init_180(void)
{
	EPD_W21_RST_0;  // Module reset   
	delay_xms(10); //At least 10ms delay 
	EPD_W21_RST_1;
	delay_xms(10); //At least 10ms delay 
	
	Epaper_READBUSY();   
	EPD_W21_WriteCMD(0x12);  //SWRESET
	Epaper_READBUSY();   
	
	EPD_W21_WriteCMD(0x3C); //BorderWavefrom
	EPD_W21_WriteDATA(0x05);
	
	EPD_W21_WriteCMD(0x01); //Driver output control      
	EPD_W21_WriteDATA((EPD_HEIGHT-1)%256);    
	EPD_W21_WriteDATA((EPD_HEIGHT-1)/256);
	EPD_W21_WriteDATA(0x00); 

	EPD_W21_WriteCMD(0x11); //data entry mode       
	EPD_W21_WriteDATA(0x02);

	EPD_W21_WriteCMD(0x44); //set Ram-X address start/end position   
	EPD_W21_WriteDATA(EPD_WIDTH/8-1);    
	EPD_W21_WriteDATA(0x00);  

	EPD_W21_WriteCMD(0x45); //set Ram-Y address start/end position          
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(0x00); 
  EPD_W21_WriteDATA((EPD_HEIGHT-1)%256);  
	EPD_W21_WriteDATA((EPD_HEIGHT-1)/256);

  EPD_W21_WriteCMD(0x18); //Read built-in temperature sensor
	EPD_W21_WriteDATA(0x80);	

	EPD_W21_WriteCMD(0x4E);   // set RAM x address count to 0;
	EPD_W21_WriteDATA(EPD_WIDTH/8-1);    
	EPD_W21_WriteCMD(0x4F);   // set RAM y address count to 0X199;    
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(0x00);
  Epaper_READBUSY();
}
// GUI initialization
void EPD_HW_Init_GUI(void)
{
	EPD_W21_RST_0;  // Module reset   
	delay_xms(10); //At least 10ms delay 
	EPD_W21_RST_1;
	delay_xms(10); //At least 10ms delay 
	
	Epaper_READBUSY();   
	EPD_W21_WriteCMD(0x12);  //SWRESET
	Epaper_READBUSY();   
		
	EPD_W21_WriteCMD(0x01); //Driver output control      
	EPD_W21_WriteDATA((EPD_HEIGHT+46-1)%256);    
	EPD_W21_WriteDATA((EPD_HEIGHT+46-1)/256);
	EPD_W21_WriteDATA(0x01);//Show mirror

	EPD_W21_WriteCMD(0x11); //data entry mode       
	EPD_W21_WriteDATA(0x01);

	EPD_W21_WriteCMD(0x44); //set Ram-X address start/end position   
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(EPD_WIDTH/8-1);   

	EPD_W21_WriteCMD(0x45); //set Ram-Y address start/end position          
	EPD_W21_WriteDATA((EPD_HEIGHT+46-1)%256);   
	EPD_W21_WriteDATA((EPD_HEIGHT+46-1)/256);
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteDATA(0x00); 

	EPD_W21_WriteCMD(0x3C); //BorderWavefrom
	EPD_W21_WriteDATA(0x05);	
	  	
  EPD_W21_WriteCMD(0x18); //Read built-in temperature sensor
	EPD_W21_WriteDATA(0x80);	

	EPD_W21_WriteCMD(0x4E);   // set RAM x address count to 0;
	EPD_W21_WriteDATA(0x00);
	EPD_W21_WriteCMD(0x4F);   // set RAM y address count to 0X199;    
	EPD_W21_WriteDATA((EPD_HEIGHT+46-1)%256);    
	EPD_W21_WriteDATA((EPD_HEIGHT+46-1)/256);
  Epaper_READBUSY();
	
}

//GUI display
void EPD_Display(unsigned char *Image)
{
	unsigned int Width, Height,i,j;
	Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
	Height = EPD_HEIGHT;

	EPD_W21_WriteCMD(0x24);
	for ( j = 0; j < Height; j++) {
			for ( i = 0; i < Width; i++) {
				 EPD_W21_WriteDATA(Image[i + j * Width]);
			}
	}
	EPD_Update();		 
}




/***********************************************************
						end file
***********************************************************/

