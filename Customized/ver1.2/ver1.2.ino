#include <avr/interrupt.h>
#include <string.h>
#include <math.h>
#define AXIS_X 1
#define AXIS_Y 2
#define AXIS_Z 3
int CUBE_SIZE = 8;

// effect_text stuff, define display string here
const int charNum = 11;
char string[charNum] = {'V','I','C','T','O','R','C','O','N','K','A'};

volatile unsigned char cube[8][8];
volatile unsigned char cubeValue;
volatile int current_layer = 0;


void setup() {

 // pinMode(A0, OUTPUT) as specified in the arduino reference didn't work. So I accessed the registers directly.
  DDRC = B00111111;
  PORTC = 0x00;
  DDRB = B00100111;
  PORTB = 0x00;
  
  // Reset any PWM configuration that the arduino may have set up automagically!
  TCCR2A = 0x02;
  TCCR2B = 0x01;
//05,16,21 - flicker free,23
//stoped at 
  

  TCCR2A |= (0x01 << WGM21); // CTC mode. clear counter on TCNT2 == OCR2A
  OCR2A = 10; // Interrupt every 25600th cpu cycle (256*100)
  TCNT2 = 0x00; // start counting at 0
  TCCR2B |= (0x01 << CS22) | (0x01 << CS21); // Start the clock with a 256 prescaler
  TIMSK2 |= (0x01 << OCIE2A);
}

ISR (TIMER2_COMPA_vect)
{
  int i;
  
  // output enable off.
  //OC -> D13
  PORTB |= B00100000;

  //Activate Layer
  PORTC = (PORTC & B11000111) | ((current_layer) << 3);
  
  for (i=0; i<8; i++)
  {
  //Load data row into a portD(ditital 0 to 7)
  //load data into shiftregister
  // PORTD = cube[current_layer][i]; //D (digital pins 0 to 7)
  cubeValue = cube[current_layer][i];
  shiftOut();
  //this goes to the flipflops
  PORTB = (PORTB & B11111000) | (B00000111 & (i+1));
  }

  // Output enable on.
  //set to 0 pin : 11. //OC -> D13 
    PORTB &=  B11011111;
  
  current_layer++;
  if (current_layer == 8)
    current_layer = 0;
}
/*
    //Pin connected to DS of 74HC595
    //dataPin = 8;A0
    //Pin connected to ST_CP of 74HC595
    //latchPin = 9;A1
    //Pin connected to SH_CP of 74HC595
    //clockPin = 10;A2
*/
void shiftOut() {
  PORTC = (PORTC & B11111000);
  int i=0;
  for (i=7; i>=0; i--)  {
    PORTC = (PORTC & B11111011);
    if ( cubeValue & (1<<i) ) {
      PORTC = (PORTC & B11111110) | (B00000001);
    }
    else {  
      PORTC = (PORTC & B11111110);
    }
    PORTC = (PORTC & B11111010) | (B00000100);
  }
  PORTC = (PORTC & B11111001) | B00000010;
}


void loop()
{
  int i,x,y,z,m,n,d;
  d=100;
   for (y=0; y<8; ++y){
    for (x=0; x<8; ++x){
      for (z=0; z<8; ++z){

        setvoxel(x,y,z); 
      }

        delay (d);
      
        for (z=0; z<8; ++z){
          clrvoxel(x,y,z); 
        }
      }
   }
     for (z=0; z<8; ++z){
       for (y=0; y<8; ++y){
          for (x=0; x<8; ++x){
        
          setvoxel(x,y,z);
           delay (d/8);
        }
      }
     }
   delay(2000);
   fill(0X00);
  
  
  while (true)
  {
    effect_intro();
    unsigned char ii;
    for (ii=0;ii<8;++ii)
      {
        effect_box_shrink_grow (1, ii%4, ii & 0x04, 1000); // (int iterations, int rot, int flip, int delay)
      }
    effect_filip_filop(75, 500); // (int iterations, int delay)
    fill(0x00);
    effect_path_bitmap(700,15,3); // (int delay, char bitmap, int iterations)
    effect_smileyspin(2,1000,15); // (int count, int delay, char bitmap)
    effect_text(1200); // (int delay)
    fireworks(10,50,500); // (int iterations, int n, int delay)
    int_ripples(200,300); // (int iterations, int delay)
    effect_cubix(150,3); // (int iterations, unsigned char cubies)
    effect_z_updown(20,1000); // (int iterations, int delay)
    effect_telcstairs(0,800,0xff); // (int invert, int delay, int val)
    effect_telcstairs(0,800,0x00); // (int invert, int delay, int val)
    effect_telcstairs(1,800,0xff); // (int invert, int delay, int val)
    effect_telcstairs(1,800,0xff); // (int invert, int delay, int val)
    fill(0x00);
    snake(200, 600, 0x01, 0x03); // (uint16_t iterations, int delay, unsigned char mode, unsigned char drawmode)
    snake(250, 600, 0x01, 0x02); // (uint16_t iterations, int delay, unsigned char mode, unsigned char drawmode)
    fill(0x00);
    fireworks(10,50,500); // (int iterations, int n, int delay)
    effect_wormsqueeze (2, AXIS_Z, -1, 100, 1000); // (int size, int axis, int direction, int iterations, int delay)
    zoom_pyramid();
    zoom_pyramid_clear(); 
    sinelines(4000,10); // (int iterations, int delay)
    linespin(1500,10); // (int iterations, int delay)
    effect_planboing(AXIS_Z, 900); // (int plane, int speed)
    effect_planboing(AXIS_Y, 900); // (int plane, int speed)
    effect_planboing(AXIS_X, 900); // (int plane, int speed)
    effect_blinky2();
    mirror_ripples(600,400); // (int iterations, int delay)
    effect_axis_updown_randsuspend(AXIS_Z, 550,5000,0); // (char axis, int delay, int sleep, int invert)
    effect_axis_updown_randsuspend(AXIS_Z, 550,5000,1); // (char axis, int delay, int sleep, int invert)
    effect_axis_updown_randsuspend(AXIS_Z, 550,5000,0); // (char axis, int delay, int sleep, int invert)
    effect_axis_updown_randsuspend(AXIS_Z, 550,5000,1); // (char axis, int delay, int sleep, int invert)
    effect_axis_updown_randsuspend(AXIS_X, 550,5000,0); // (char axis, int delay, int sleep, int invert)
    effect_axis_updown_randsuspend(AXIS_X, 550,5000,1); // (char axis, int delay, int sleep, int invert)
    effect_axis_updown_randsuspend(AXIS_Y, 550,5000,0); // (char axis, int delay, int sleep, int invert)
    effect_axis_updown_randsuspend(AXIS_Y, 550,5000,1); // (char axis, int delay, int sleep, int invert)
    effect_rand_patharound(200,500); // (int iterations, int delay)
    effect_random_filler(75,1); // (int delay, int state)
    delay(1000);
    effect_random_filler(75,0); // (int delay, int state)
    zoom_pyramid();
    zoom_pyramid_clear();
    effect_rain(100); // (int iterations)
    side_ripples (300, 500); // (int iterations, int delay)
    effect_text_up(1200); // (int delayt)
    effect_random_sparkle();
    quad_ripples (600,300); // (int iterations, int delay)
    effect_boxside_randsend_parallel (AXIS_X, 0, 150, 1); // (char axis, int origin, int delay, int mode)
    effect_boxside_randsend_parallel (AXIS_X, 1, 150, 1); // (char axis, int origin, int delay, int mode)
    effect_boxside_randsend_parallel (AXIS_Y, 0, 150, 1); // (char axis, int origin, int delay, int mode)
    effect_boxside_randsend_parallel (AXIS_Y, 1, 150, 1); // (char axis, int origin, int delay, int mode)
    effect_boxside_randsend_parallel (AXIS_Z, 0, 150, 1); // (char axis, int origin, int delay, int mode)
    effect_boxside_randsend_parallel (AXIS_Z, 1, 150, 1); // (char axis, int origin, int delay, int mode)
    
  }
}


// ==========================================================================================
//   Effect functions ported from other code - Porting by TheLazyMastermind
// ==========================================================================================

void effect_cubix(int iterations, unsigned char cubies)
{
	// cube array:
	// Stupid name to prevent confusion with cube[][]!
	// 0=pos
	// 1=dir (0 stopped 1,2 or 4 move in X, Y or Z)
	// 2=inc or dec in direction (2=inc, 0=dec)
	// 3=countdown to new movement
	// 4=x, 5=y, 6=z
	unsigned char qubes[cubies][7];
	int ii; // iteration counter
	unsigned char i,j,diridx,newdir;
	unsigned char runstate=2;
	//	Initialise qubes array
	for (i=0;i<cubies;++i)
	{
		qubes[i][0]=i;// position = i
		qubes[i][1]=0;// static
		qubes[i][3]=rand()&0x0f;// 0-15
		// Hack in the X,Y and Z positions
		qubes[i][4]=4*(i&0x01);
		qubes[i][5]=2*(i&0x02);
		qubes[i][6]=(i&0x04);
	}
	// Main loop
	ii=iterations;
	while(runstate)
	{
		fill(0x00);
		for (i=0;i<cubies;++i)
		{
			// Use a pointer to simplify array indexing
			// qube[0..7] = qubes[i][0..7]
			unsigned char *qube=&qubes[i][0];
			if (qube[1]) //moving
			{
				diridx=3+qube[1];//4,5 or 7
				if (diridx==7) diridx=6;
				qube[diridx]+=qube[2]-1;
				if ((qube[diridx]==0)||(qube[diridx]==4))
				{
					// XOR old pos and dir to get new pos.
					qube[0]=qube[0]^qube[1];
					qube[1]=0;// Stop moving!
					qube[3]=rand()&0x0f; // countdown to next move 0-15
					if(runstate==1)
						if (qube[0]<5)
							qube[3]*=4;// Make lower qubes move very slowly to finish
				}
			}
			else // not moving
			{
				if (qube[3])// counting down
					--qube[3];
				else // ready to move
				{
					newdir=(1<<(rand()%3));//1,2 or 4
					diridx=qube[0]^newdir;
					for (j=0;j<cubies;++j)// check newdir is safe to move to
					{
						if ((diridx==qubes[j][0])||(diridx==(qubes[j][0]^qubes[j][1])))
						{
							newdir=0;
							qube[3]=5;
						}
					}

					if (newdir)
					{
						diridx=3+newdir;
						if (diridx==7) diridx=6;
						if (qube[diridx])// should be 4 or 0
							qube[2]=0; // dec if at 4
						else
						{
							qube[2]=2; // inc if at 0
							if(runstate==1)// Try to make qubes go home
								if ((diridx>4)&&(qube[0]<4))
									newdir=0;//Don't allow qubes on bottom row to move up or back
						}
					}
					qube[1]=newdir;
				}
			}
			//if (i&0x01)//odd number
				box_filled(qube[4],qube[5],qube[6],qube[4]+3,qube[5]+3,qube[6]+3);
			//else
			//	box_wireframe(qube[4],qube[5],qube[6],qube[4]+3,qube[5]+3,qube[6]+3);
		} // i loop
		delay_ms(800);
		if(runstate==2)// If normal running
		{
			if(!(--ii))// decrement iteration and check for zero
				runstate=1;// If zero go to homing
		}
		else//runstate at 1
		{
			diridx=0;
			for(j=0;j<cubies;++j)
				if (qubes[j][0]+1>cubies) diridx=1;// Any cube not at home
			if (!diridx)
				runstate=0;
		}
	} // Main loop
}

void effect_filip_filop(int iterations, int FF_DELAY)
{
//	LUT_START // Macro
        unsigned char LUT[65];
        init_LUT(LUT);
	unsigned char now_plane=0;//start at top
	unsigned char next_plane;
	int delay=FF_DELAY;
	int i;//iteration counter
	for (i=iterations;i;--i)
	{
		next_plane=rand()%6; //0-5
		// Check that not the same, and that:
		// 0/1 2/3 4/5 pairs do not exist.
		if ((next_plane&0x06)==(now_plane&0x06))
			next_plane=(next_plane+3)%6;
		effect_plane_flip(LUT,now_plane,next_plane,delay);
		now_plane=next_plane;
	}
}

void effect_smileyspin (int count, int delay, char bitmap)
{
	unsigned char dybde[] = {0,1,2,3,4,5,6,7,1,1,2,3,4,5,6,6,2,2,3,3,4,4,5,5,3,3,3,3,4,4,4,4};
	int d = 0;
	int flip = 0;
	int x, y, off;
	for(int i = 0; i<count; i++)
	{
		flip = 0;
		d = 0;
		off = 0;
		// front:
		for (int s=0;s<7;s++){
			if(!flip){
				off++;
				if (off == 4){
					flip = 1;
					off = 0;
				}
			} else {
				off++;
			}
		        for (x=0; x<8; x++)
        		{
				d = 0;
                		for (y=0; y<8; y++)
	                	{
					if (font_getbitmappixel ( bitmap, 7-x, y)){
						if (!flip)
							setvoxel(y,dybde[8 * off + d++],x);
						else
							setvoxel(y,dybde[31 - 8 * off - d++],x);
					} else {
						d++;
					}
				}
			}
			delay_ms(delay);
			fill(0x00);
		}

		// side:
		off = 0;
		flip = 0;
		d = 0;
		for (int s=0;s<7;s++){
			if(!flip){
				off++;
				if (off == 4){
					flip = 1;
					off = 0;
				}
			} else {
				off++;
			}
		        for (x=0; x<8; x++)
        		{
				d = 0;
                		for (y=0; y<8; y++)
	                	{
					if (font_getbitmappixel ( bitmap, 7-x, y)){
						if (!flip)
							setvoxel(dybde[8 * off + d++], 7 - y,x);
						else
							setvoxel(dybde[31 - 8 * off - d++],7 - y,x);
					} else {
						d++;
					}
				}
			}
			delay_ms(delay);
			fill(0x00);
		}


		flip = 0;
		d = 0;
		off = 0;
		// back:
		for (int s=0;s<7;s++){
			if(!flip){
				off++;
				if (off == 4){
					flip = 1;
					off = 0;
				}
			} else {
				off++;
			}
		        for (x=0; x<8; x++)
        		{
				d = 0;
                		for (y=0; y<8; y++)
	                	{
					if (font_getbitmappixel ( bitmap, 7-x, 7-y)){
						if (!flip)
							setvoxel(y,dybde[8 * off + d++],x);
						else
							setvoxel(y,dybde[31 - 8 * off - d++],x);
					} else {
						d++;
					}
				}
			}
			delay_ms(delay);
			fill(0x00);
		}

		// other side:
		off = 0;
		flip = 0;
		d = 0;
		for (int s=0;s<7;s++){
			if(!flip){
				off++;
				if (off == 4){
					flip = 1;
					off = 0;
				}
			} else {
				off++;
			}
		        for (x=0; x<8; x++)
        		{
				d = 0;
                		for (y=0; y<8; y++)
	                	{
					if (font_getbitmappixel ( bitmap, 7-x, 7-y)){
						if (!flip)
							setvoxel(dybde[8 * off + d++], 7 - y,x);
						else
							setvoxel(dybde[31 - 8 * off - d++],7 - y,x);
					} else {
						d++;
					}
				}
			}
			delay_ms(delay);
			fill(0x00);
		}

	}
}

void effect_z_updown (int iterations, int delay)
{
	unsigned char positions[64];
	unsigned char destinations[64];

	int i,y,move,px;
	
	for (i=0; i<64; ++i)
	{
		positions[i] = 4;
		destinations[i] = rand()%8;
	}

	for (i=0; i<8; ++i)
	{
		effect_z_updown_move(positions, destinations, AXIS_Z);
		delay_ms(delay);
	}
	
	for (i=0;i<iterations;++i)
	{
		for (move=0;move<8;++move)
		{
			effect_z_updown_move(positions, destinations, AXIS_Z);
			delay_ms(delay);
		}

		delay_ms(delay*4);


		for (y=0;y<10;++y)
		{
				destinations[rand()%64] = rand()%8;
		}
		
	}

}

void effect_z_updown_move (unsigned char positions[64], unsigned char destinations[64], char axis)
{
	int px;
	for (px=0; px<64; ++px)
	{
		if (positions[px]<destinations[px])
		{
			++positions[px];
		}
		if (positions[px]>destinations[px])
		{
			--positions[px];
		}
	}
		
	draw_positions_axis (AXIS_Z, positions,0);
}

void effect_path_bitmap (int delay, char bitmap, int iterations)
{
	int z, i, ii;
	z = 4;
	unsigned char path[28];
	font_getpath(0,path,28);
	
	for (i=0; i < iterations; ++i)
	{	
		for (ii=0;ii<8;++ii)
		{		
			for (z=0;z<8;++z)
			{
				if (font_getbitmappixel(bitmap,(7-z),ii))
				{
					setvoxel(0,7,z);
				} else
				{
					clrvoxel(0,7,z);
				}
				
			}
			delay_ms(delay);
			effect_pathmove(path, 28);
		}
	
		for (ii=0;ii<20;++ii)
		{
			delay_ms(delay);
			effect_pathmove(path, 28);
		}
	}
	for (ii=0;ii<10;++ii)
	{
		delay_ms(delay);
		effect_pathmove(path, 28);
	}
}

void snake(uint16_t iterations, int delay, unsigned char mode, unsigned char drawmode)
{
	fill(0x00);		// Blank the cube

	int x, y, z;		// Current coordinates for the point
	int dx, dy, dz;	// Direction of movement
	int lol, i;		// lol?
	unsigned char crash_x, crash_y, crash_z;

	y = rand()%8;
	x = rand()%8;
	z = rand()%8;

	// Coordinate array for the snake.
	int snake[8][3];
	for (i=0;i<8;++i)
	{
		snake[i][0] = x;
		snake[i][1] = y;
		snake[i][2] = z;
	}
	
	
	dx = 1;
	dy = 1;
	dz = 1;
	
	while(iterations)
	{
		crash_x = 0;
		crash_y = 0;
		crash_z = 0;
	

		// Let's mix things up a little:
		if (rand()%3 == 0)
		{
			// Pick a random axis, and set the speed to a random number.
			lol = rand()%3;
			if (lol == 0)
				dx = rand()%3 - 1;
			
			if (lol == 1)
				dy = rand()%3 - 1;
				
			if (lol == 2)
				dz = rand()%3 - 1;
		}

	    // The point has reached 0 on the x-axis and is trying to go to -1
        // aka a crash
		if (dx == -1 && x == 0)
		{
			crash_x = 0x01;
			if (rand()%3 == 1)
			{
				dx = 1;
			} else
			{
				dx = 0;
			}
		}
		
        // y axis 0 crash
		if (dy == -1 && y == 0)
		{
			crash_y = 0x01;
			if (rand()%3 == 1)
			{
				dy = 1;
			} else
			{
				dy = 0;
			}
		}
		
        // z axis 0 crash
		if (dz == -1 && z == 0)
		{
			crash_z = 0x01;
			if (rand()%3 == 1)
			{
				dz = 1;
			} else
			{
				dz = 0;
			}
		}
	    
        // x axis 7 crash
		if (dx == 1 && x == 7)
		{
			crash_x = 0x01;
			if (rand()%3 == 1)
			{
				dx = -1;
			} else
			{
				dx = 0;
			}
		}
		
        // y axis 7 crash
		if (dy == 1 && y == 7)
		{
			crash_y = 0x01;
			if (rand()%3 == 1)
			{
				dy = -1;
			} else
			{
				dy = 0;
			}
		}
		
        // z azis 7 crash
		if (dz == 1 && z == 7)
		{
			crash_z = 0x01;
			if (rand()%3 == 1)
			{
				dz = -1;
			} else
			{
				dz = 0;
			}
		}
		
		// mode bit 0 sets crash action enable
		if (mode | 0x01)
		{
			if (crash_x)
			{
				if (dy == 0)
				{
					if (y == 7)
					{
						dy = -1;
					} else if (y == 0)
					{
						dy = +1;
					} else
					{
						if (rand()%2 == 0)
						{
							dy = -1;
						} else
						{
							dy = 1;
						}
					}
				}
				if (dz == 0)
				{
					if (z == 7)
					{
						dz = -1;
					} else if (z == 0)
					{
						dz = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dz = -1;
						} else
						{
							dz = 1;
						}
					}	
				}
			}
			
			if (crash_y)
			{
				if (dx == 0)
				{
					if (x == 7)
					{
						dx = -1;
					} else if (x == 0)
					{
						dx = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dx = -1;
						} else
						{
							dx = 1;
						}
					}
				}
				if (dz == 0)
				{
					if (z == 3)
					{
						dz = -1;
					} else if (z == 0)
					{
						dz = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dz = -1;
						} else
						{
							dz = 1;
						}
					}	
				}
			}
			
			if (crash_z)
			{
				if (dy == 0)
				{
					if (y == 7)
					{
						dy = -1;
					} else if (y == 0)
					{
						dy = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dy = -1;
						} else
						{
							dy = 1;
						}
					}	
				}
				if (dx == 0)
				{
					if (x == 7)
					{
						dx = -1;
					} else if (x == 0)
					{
						dx = 1;
					} else
					{
						if (rand()%2 == 0)
						{
							dx = -1;
						} else
						{
							dx = 1;
						}
					}	
				}
			}
		}
		
		// mode bit 1 sets corner avoid enable
		if (mode | 0x02)
		{
			if (	// We are in one of 8 corner positions
				(x == 0 && y == 0 && z == 0) ||
				(x == 0 && y == 0 && z == 7) ||
				(x == 0 && y == 7 && z == 0) ||
				(x == 0 && y == 7 && z == 7) ||
				(x == 7 && y == 0 && z == 0) ||
				(x == 7 && y == 0 && z == 7) ||
				(x == 7 && y == 7 && z == 0) ||
				(x == 7 && y == 7 && z == 7)
			)
			{
				// At this point, the voxel would bounce
				// back and forth between this corner,
				// and the exact opposite corner
				// We don't want that!
			
				// So we alter the trajectory a bit,
				// to avoid corner stickyness
				lol = rand()%3;
				if (lol == 0)
					dx = 0;
				
				if (lol == 1)
					dy = 0;
					
				if (lol == 2)
					dz = 0;
			}
		}

        // one last sanity check
        if (x == 0 && dx == -1)
            dx = 1;
	
        if (y == 0 && dy == -1)
            dy = 1;
	
        if (z == 0 && dz == -1)
            dz = 1;
	
        if (x == 7 && dx == 1)
            dx = -1;
	
        if (y == 7 && dy == 1)
            dy = -1;
	
        if (z == 7 && dz == 1)
            dz = -1;
	
	
		// Finally, move the voxel.
		x = x + dx;
		y = y + dy;
		z = z + dz;
		
		if (drawmode == 0x01) // show one voxel at time
		{
			setvoxel(x,y,z);
			delay_ms(delay);
			clrvoxel(x,y,z);	
		} else if (drawmode == 0x02) // flip the voxel in question
		{
			flpvoxel(x,y,z);
			delay_ms(delay);
		} if (drawmode == 0x03) // draw a snake
		{
			for (i=7;i>=0;--i)
			{
				snake[i][0] = snake[i-1][0];
				snake[i][1] = snake[i-1][1];
				snake[i][2] = snake[i-1][2];
			}
			snake[0][0] = x;
			snake[0][1] = y;
			snake[0][2] = z;
				
			for (i=0;i<8;++i)
			{
				setvoxel(snake[i][0],snake[i][1],snake[i][2]);
			}
			delay_ms(delay);
			for (i=0;i<8;++i)
			{
				clrvoxel(snake[i][0],snake[i][1],snake[i][2]);
			}
		}
		
	
		--iterations;
	}
}

void effect_box_shrink_grow (int iterations, int rot, int flip, int delay)
{
	int x, i, xyz;
	for (x=0;x<iterations;++x)
	{
		for (i=0;i<16;++i)
		{
            xyz = 7-i; // This reverses counter i between 0 and 7.
            if (i > 7) 
                xyz = i-8; // at i > 7, i 8-15 becomes xyz 0-7.
            
            fill(0x00); delay_ms(1);
            cli(); // disable interrupts while the cube is being rotated
			box_wireframe(0,0,0,xyz,xyz,xyz);

            if (flip > 0) // upside-down
                mirror_z();

            if (rot == 1 || rot == 3)
                mirror_y();

            if (rot == 2 || rot == 3)
                mirror_x();
            
            sei(); // enable interrupts
			delay_ms(delay);
	        fill(0x00);
        }
    }
}


// ==========================================================================================
// Included Effect Functions
// ==========================================================================================

void fireworks (int iterations, int n, int delay)
{
	fill(0x00);

	int i,f,e;

	float origin_x = 3;
	float origin_y = 3;
	float origin_z = 3;

	int rand_y, rand_x, rand_z;

	float slowrate, gravity;

	// Particles and their position, x,y,z and their movement, dx, dy, dz
	float particles[n][6];

	for (i=0; i<iterations; ++i)
	{

		origin_x = rand()%4;
		origin_y = rand()%4;
		origin_z = rand()%2;
		origin_z +=5;
        origin_x +=2;
        origin_y +=2;

		// shoot a particle up in the air
		for (e=0;e<origin_z;++e)
		{
			setvoxel(origin_x,origin_y,e);
			delay_ms(600+500*e);
			fill(0x00);
		}

		// Fill particle array
		for (f=0; f<n; ++f)
		{
			// Position
			particles[f][0] = origin_x;
			particles[f][1] = origin_y;
			particles[f][2] = origin_z;
			
			rand_x = rand()%200;
			rand_y = rand()%200;
			rand_z = rand()%200;

			// Movement
			particles[f][3] = 1-(float)rand_x/100; // dx
			particles[f][4] = 1-(float)rand_y/100; // dy
			particles[f][5] = 1-(float)rand_z/100; // dz
		}

		// explode
		for (e=0; e<25; ++e)
		{
			slowrate = 1+tan((e+0.1)/20)*10;
			
			gravity = tan((e+0.1)/20)/2;

			for (f=0; f<n; ++f)
			{
				particles[f][0] += particles[f][3]/slowrate;
				particles[f][1] += particles[f][4]/slowrate;
				particles[f][2] += particles[f][5]/slowrate;
				particles[f][2] -= gravity;

				setvoxel(particles[f][0],particles[f][1],particles[f][2]);


			}

			delay_ms(delay);
			fill(0x00);
		}

	}

}

void quad_ripples(int iterations, int delay)
{
	// 16 values for square root of a^2+b^2.  index a*4+b = 10*sqrt
	// This gives the distance to 3.5,3.5 from the point
	unsigned char sqrt_LUT[]={49,43,38,35,43,35,29,26,38,29,21,16,35,25,16,7};
	//LUT_START // Macro from new tottymath.  Commented and replaced with full code
	unsigned char LUT[65];
	init_LUT(LUT);
	int i;
	unsigned char x,y,height,distance;
	for (i=0;i<iterations*4;i+=4)
	{
		fill(0x00);
		for (x=0;x<4;++x)
			for(y=0;y<4;++y)
			{
				// x+y*4 gives no. from 0-15 for sqrt_LUT
				distance=sqrt_LUT[x+y*4];// distance is 0-50 roughly
				// height is sin of distance + iteration*4
				//height=4+(LUT,distance+i)/52;
				height=(196+totty_sin(LUT,distance+i))/49;
				// Use 4-way mirroring to save on calculations
				setvoxel(x,y,height);
				setvoxel(7-x,y,height);
				setvoxel(x,7-y,height);
				setvoxel(7-x,7-y,height);
				setvoxel(x,y,7-height);
				setvoxel(7-x,y,7-height);
				setvoxel(x,7-y,7-height);
				setvoxel(7-x,7-y,7-height);
				setvoxel(x,height,y);
				setvoxel(7-x,height,y);
				setvoxel(x,height,7-y);
				setvoxel(7-x,height,7-y);
				setvoxel(x,7-height,y);
				setvoxel(7-x,7-height,y);
				setvoxel(x,7-height,7-y);
				setvoxel(7-x,7-height,7-y);


			}
		delay_ms(delay);
	}
}



// **********************************************************

void effect_random_sparkle_flash (int iterations, int voxels, int delay)
{
	int i;
	int v;
	for (i = 0; i < iterations; ++i)
	{
		for (v=0;v<=voxels;++v)
			setvoxel(rand()%8,rand()%8,rand()%8);
			
		delay_ms(delay);
		fill(0x00);
	}
}

// blink 1 random voxel, blink 2 random voxels..... blink 20 random voxels
// and back to 1 again.
void effect_random_sparkle (void)
{
	int i;
	
	for (i=1;i<20;++i)
	{
		effect_random_sparkle_flash(5,i,200);
	}
	
	for (i=20;i>=1;--i)
	{
		effect_random_sparkle_flash(5,i,200);
	}
	
}

int effect_telcstairs_do(int x, int val, int delay)
{
	int y,z;

	for(y = 0, z = x; y <= z; ++y, --x)
	{
		if(x < CUBE_SIZE && y < CUBE_SIZE)
		{
			cube[x][y] = val;
		}
	}
	delay_ms(delay);
	return z;
}

void effect_telcstairs (int invert, int delay, int val)
{
	int x;

	if(invert)
	{
		for(x = CUBE_SIZE*2; x >= 0; --x)
		{
			x = effect_telcstairs_do(x,val,delay);
		}
	}
	else
	{
		for(x = 0; x < CUBE_SIZE*2; ++x)
		{
			x = effect_telcstairs_do(x,val,delay);
		}
	}
}



void draw_positions_axis (char axis, unsigned char positions[64], int invert)
{
	int x, y, p;
	
	fill(0x00);
	
	for (x=0; x<8; ++x)
	{
		for (y=0; y<8; ++y)
		{
			if (invert)
			{
				p = (7-positions[(x*8)+y]);
			} else
			{
				p = positions[(x*8)+y];
			}
		
			if (axis == AXIS_Z)
				setvoxel(x,y,p);
				
			if (axis == AXIS_Y)
				setvoxel(x,p,y);
				
			if (axis == AXIS_X)
				setvoxel(p,y,x);
		}
	}
	
}

void effect_wormsqueeze (int size, int axis, int direction, int iterations, int delay)
{
	int x, y, i,j,k, dx, dy;
	int cube_size;
	int origin = 0;
	
	if (direction == -1)
		origin = 7;
	
	cube_size = 8-(size-1);
	
	x = rand()%cube_size;
	y = rand()%cube_size;
	
	for (i=0; i<iterations; ++i)
	{
		dx = ((rand()%3)-1);
		dy = ((rand()%3)-1);
	
		if ((x+dx) > 0 && (x+dx) < cube_size)
			x += dx;
			
		if ((y+dy) > 0 && (y+dy) < cube_size)
			y += dy;
	
		shift(axis, direction);
		

		for (j=0; j<size;++j)
		{
			for (k=0; k<size;++k)
			{
				if (axis == AXIS_Z)
					setvoxel(x+j,y+k,origin);
					
				if (axis == AXIS_Y)
					setvoxel(x+j,origin,y+k);
					
				if (axis == AXIS_X)
					setvoxel(origin,y+j,x+k);
			}
		}
		
		delay_ms(delay);
	}
}



void sinelines (int iterations, int delay)
{
	int i,x;

	float left, right, sine_base, x_dividor,ripple_height;

	for (i=0; i<iterations; ++i)
	{
		for (x=0; x<8 ;++x)
		{
			x_dividor = 2 + sin((float)i/100)+1;
			ripple_height = 3 + (sin((float)i/200)+1)*6;

			sine_base = (float) i/40 + (float) x/x_dividor;

			left = 4 + sin(sine_base)*ripple_height;
			right = 4 + cos(sine_base)*ripple_height;
			right = 7-left;

			//printf("%i %i \n", (int) left, (int) right);

			line_3d(0-3, x, (int) left, 7+3, x, (int) right);
			//line_3d((int) right, 7, x);
		}
	
	// delay_ms(delay);
	fill(0x00);
	}
}


void effect_boxside_randsend_parallel (char axis, int origin, int delay, int mode)
{
	int i;
	int done;
	unsigned char cubepos[64];
	unsigned char pos[64];
	int notdone = 1;
	int notdone2 = 1;
	int sent = 0;
	
	for (i=0;i<64;++i)
	{
		pos[i] = 0;
	}
	
	while (notdone)
	{
		if (mode == 1)
		{
			notdone2 = 1;
			while (notdone2 && sent<64)
			{
				i = rand()%64;
				if (pos[i] == 0)
				{
					++sent;
					pos[i] += 1;
					notdone2 = 0;
				}
			}
		} else if (mode == 2)
		{
			if (sent<64)
			{
				pos[sent] += 1;
				++sent;
			}
		}
		
		done = 0;
		for (i=0;i<64;++i)
		{
			if (pos[i] > 0 && pos[i] <7)
			{
				pos[i] += 1;
			}
				
			if (pos[i] == 7)
				++done;
		}
		
		if (done == 64)
			notdone = 0;
		
		for (i=0;i<64;++i)
		{
			if (origin == 0)
			{
				cubepos[i] = pos[i];
			} else
			{
				cubepos[i] = (7-pos[i]);
			}
		}
		
		
		delay_ms(delay);
		draw_positions_axis(axis,cubepos,0);

	}
	
}


void effect_rain (int iterations)
{
	int i, ii;
	int rnd_x;
	int rnd_y;
	int rnd_num;
	
	for (ii=0;ii<iterations;++ii)
	{
		rnd_num = rand()%4;
		
		for (i=0; i < rnd_num;++i)
		{
			rnd_x = rand()%8;
			rnd_y = rand()%8;
			setvoxel(rnd_x,rnd_y,7);
		}
		
		delay_ms(1000);
		shift(AXIS_Z,-1);
	}
}

// Set or clear exactly 512 voxels in a random order.
void effect_random_filler (int delay, int state)
{
	int x,y,z;
	int loop = 0;
	
	
	if (state == 1)
	{
		fill(0x00);
	} else
	{
		fill(0xff);
	}
	
	while (loop<511)
	{
		x = rand()%8;
		y = rand()%8;
		z = rand()%8;

		if ((state == 0 && getvoxel(x,y,z) == 0x01) || (state == 1 && getvoxel(x,y,z) == 0x00))
		{
			altervoxel(x,y,z,state);
			delay_ms(delay);
			++loop;
		}	
	}
}

void effect_axis_updown_randsuspend (char axis, int delay, int sleep, int invert)
{
	unsigned char positions[64];
	unsigned char destinations[64];

	int i,px;
	
    // Set 64 random positions
	for (i=0; i<64; ++i)
	{
		positions[i] = 0; // Set all starting positions to 0
		destinations[i] = rand()%8;
	}

    // Loop 8 times to allow destination 7 to reach all the way
	for (i=0; i<8; ++i)
	{
        // For every iteration, move all position one step closer to their destination
		for (px=0; px<64; ++px)
		{
			if (positions[px]<destinations[px])
			{
				++positions[px];
			}
		}
        // Draw the positions and take a nap
		draw_positions_axis (axis, positions,invert);
		delay_ms(delay);
	}
	
    // Set all destinations to 7 (opposite from the side they started out)
	for (i=0; i<64; ++i)
	{
		destinations[i] = 7;
	}
	
    // Suspend the positions in mid-air for a while
	delay_ms(sleep);
	
    // Then do the same thing one more time
	for (i=0; i<8; ++i)
	{
		for (px=0; px<64; ++px)
		{
			if (positions[px]<destinations[px])
			{
				++positions[px];
			}
			if (positions[px]>destinations[px])
			{
				--positions[px];
			}
		}
		draw_positions_axis (axis, positions,invert);
		delay_ms(delay);
	}
}



void effect_blinky2()
{
	int i,r;
	fill(0x00);
	
	for (r=0;r<2;++r)
	{
		i = 750;
		while (i>0)
		{
			fill(0x00);
			delay_ms(i);
			
			fill(0xff);
			delay_ms(100);
			
			i = i - (15+(1000/(i/10)));
		}
		
		delay_ms(1000);
		
		i = 750;
		while (i>0)
		{
			fill(0x00);
			delay_ms(751-i);
			
			fill(0xff);
			delay_ms(100);
			
			i = i - (15+(1000/(i/10)));
		}
	}

}

// Draw a plane on one axis and send it back and forth once.
void effect_planboing (int plane, int speed)
{
	int i;
	for (i=0;i<8;++i)
	{
		fill(0x00);
        setplane(plane, i);
		delay_ms(speed);
	}
	
	for (i=7;i>=0;--i)
	{
		fill(0x00);
        setplane(plane,i);
		delay_ms(speed);
	}
}


//********************************************************

// ==========================================================================================
//   Draw functions
// ==========================================================================================


// Set a single voxel to ON
void setvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
		cube[z][y] |= (1 << x);
}


// Set a single voxel to ON
void clrvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
		cube[z][y] &= ~(1 << x);
}



// This function validates that we are drawing inside the cube.
unsigned char inrange(int x, int y, int z)
{
	if (x >= 0 && x < 8 && y >= 0 && y < 8 && z >= 0 && z < 8)
	{
		return 0x01;
	} else
	{
		// One of the coordinates was outside the cube.
		return 0x00;
	}
}

// Get the current status of a voxel
unsigned char getvoxel(int x, int y, int z)
{
	if (inrange(x,y,z))
	{
		if (cube[z][y] & (1 << x))
		{
			return 0x01;
		} else
		{
			return 0x00;
		}
	} else
	{
		return 0x00;
	}
}

// In some effect we want to just take bool and write it to a voxel
// this function calls the apropriate voxel manipulation function.
void altervoxel(int x, int y, int z, int state)
{
	if (state == 1)
	{
		setvoxel(x,y,z);
	} else
	{
		clrvoxel(x,y,z);
	}
}

// Flip the state of a voxel.
// If the voxel is 1, its turned into a 0, and vice versa.
void flpvoxel(int x, int y, int z)
{
	if (inrange(x, y, z))
		cube[z][y] ^= (1 << x);
}

// Makes sure x1 is alwas smaller than x2
// This is usefull for functions that uses for loops,
// to avoid infinite loops
void argorder(int ix1, int ix2, int *ox1, int *ox2)
{
	if (ix1>ix2)
	{
		int tmp;
		tmp = ix1;
		ix1= ix2;
		ix2 = tmp;
	}
	*ox1 = ix1;
	*ox2 = ix2;
}

// Sets all voxels along a X/Y plane at a given point
// on axis Z
void setplane_z (int z)
{
	int i;
	if (z>=0 && z<8)
	{
		for (i=0;i<8;++i)
			cube[z][i] = 0xff;
	}
}

// Clears voxels in the same manner as above
void clrplane_z (int z)
{
	int i;
	if (z>=0 && z<8)
	{
		for (i=0;i<8;++i)
			cube[z][i] = 0x00;
	}
}

void setplane_x (int x)
{
	int z;
	int y;
	if (x>=0 && x<8)
	{
		for (z=0;z<8;++z)
		{
			for (y=0;y<8;++y)
			{
				cube[z][y] |= (1 << x);
			}
		}
	}
}

void clrplane_x (int x)
{
	int z;
	int y;
	if (x>=0 && x<8)
	{
		for (z=0;z<8;++z)
		{
			for (y=0;y<8;++y)
			{
				cube[z][y] &= ~(1 << x);
			}
		}
	}
}

void setplane_y (int y)
{
	int z;
	if (y>=0 && y<8)
	{
		for (z=0;z<8;++z)
			cube[z][y] = 0xff;
	} 
}

void clrplane_y (int y)
{
	int z;
	if (y>=0 && y<8)
	{
		for (z=0;z<8;++z)
			cube[z][y] = 0x00; 
	}
}

void setplane (char axis, unsigned char i)
{
    switch (axis)
    {
        case AXIS_X:
            setplane_x(i);
            break;
        
       case AXIS_Y:
            setplane_y(i);
            break;

       case AXIS_Z:
            setplane_z(i);
            break;
    }
}

void clrplane (char axis, unsigned char i)
{
    switch (axis)
    {
        case AXIS_X:
            clrplane_x(i);
            break;
        
       case AXIS_Y:
            clrplane_y(i);
            break;

       case AXIS_Z:
            clrplane_z(i);
            break;
    }
}

// Fill a value into all 64 byts of the cube buffer
// Mostly used for clearing. fill(0x00)
// or setting all on. fill(0xff)
void fill (unsigned char pattern)
{
	int z;
	int y;
	for (z=0;z<8;++z)
	{
		for (y=0;y<8;++y)
		{
			cube[z][y] = pattern;
		}
	}
}



// Draw a box with all walls drawn and all voxels inside set
void box_filled(int x1, int y1, int z1, int x2, int y2, int z2)
{
	int iy;
	int iz;

	argorder(x1, x2, &x1, &x2);
	argorder(y1, y2, &y1, &y2);
	argorder(z1, z2, &z1, &z2);

	for (iz=z1;iz<=z2;++iz)
	{
		for (iy=y1;iy<=y2;++iy)
		{
			cube[iz][iy] |= byteline(x1,x2);
		}
	}

}

// Darw a hollow box with side walls.
void box_walls(int x1, int y1, int z1, int x2, int y2, int z2)
{
	int iy;
	int iz;
	
	argorder(x1, x2, &x1, &x2);
	argorder(y1, y2, &y1, &y2);
	argorder(z1, z2, &z1, &z2);

	for (iz=z1;iz<=z2;++iz)
	{
		for (iy=y1;iy<=y2;++iy)
		{	
			if (iy == y1 || iy == y2 || iz == z1 || iz == z2)
			{
				cube[iz][iy] = byteline(x1,x2);
			} else
			{
				cube[iz][iy] |= ((0x01 << x1) | (0x01 << x2));
			}
		}
	}

}

// Draw a wireframe box. This only draws the corners and edges,
// no walls.
void box_wireframe(int x1, int y1, int z1, int x2, int y2, int z2)
{
	int iy;
	int iz;

	argorder(x1, x2, &x1, &x2);
	argorder(y1, y2, &y1, &y2);
	argorder(z1, z2, &z1, &z2);

	// Lines along X axis
	cube[z1][y1] = byteline(x1,x2);
	cube[z1][y2] = byteline(x1,x2);
	cube[z2][y1] = byteline(x1,x2);
	cube[z2][y2] = byteline(x1,x2);

	// Lines along Y axis
	for (iy=y1;iy<=y2;++iy)
	{
		setvoxel(x1,iy,z1);
		setvoxel(x1,iy,z2);
		setvoxel(x2,iy,z1);
		setvoxel(x2,iy,z2);
	}

	// Lines along Z axis
	for (iz=z1;iz<=z2;++iz)
	{
		setvoxel(x1,y1,iz);
		setvoxel(x1,y2,iz);
		setvoxel(x2,y1,iz);
		setvoxel(x2,y2,iz);
	}

}

// Returns a byte with a row of 1's drawn in it.
// byteline(2,5) gives 0b00111100
char byteline (int start, int end)
{
	return ((0xff<<start) & ~(0xff<<(end+1)));
}

// Flips a byte 180 degrees.
// MSB becomes LSB, LSB becomes MSB.
char flipbyte (char byte)
{
	char flop = 0x00;

	flop = (flop & 0b11111110) | (0b00000001 & (byte >> 7));
	flop = (flop & 0b11111101) | (0b00000010 & (byte >> 5));
	flop = (flop & 0b11111011) | (0b00000100 & (byte >> 3));
	flop = (flop & 0b11110111) | (0b00001000 & (byte >> 1));
	flop = (flop & 0b11101111) | (0b00010000 & (byte << 1));
	flop = (flop & 0b11011111) | (0b00100000 & (byte << 3));
	flop = (flop & 0b10111111) | (0b01000000 & (byte << 5));
	flop = (flop & 0b01111111) | (0b10000000 & (byte << 7));
	return flop;
}

// Draw a line between any coordinates in 3d space.
// Uses integer values for input, so dont expect smooth animations.
void line(int x1, int y1, int z1, int x2, int y2, int z2)
{
	float xy;	// how many voxels do we move on the y axis for each step on the x axis
	float xz;	// how many voxels do we move on the y axis for each step on the x axis 
	unsigned char x,y,z;
	unsigned char lasty,lastz;

	// We always want to draw the line from x=0 to x=7.
	// If x1 is bigget than x2, we need to flip all the values.
	if (x1>x2)
	{
		int tmp;
		tmp = x2; x2 = x1; x1 = tmp;
		tmp = y2; y2 = y1; y1 = tmp;
		tmp = z2; z2 = z1; z1 = tmp;
	}

	
	if (y1>y2)
	{
		xy = (float)(y1-y2)/(float)(x2-x1);
		lasty = y2;
	} else
	{
		xy = (float)(y2-y1)/(float)(x2-x1);
		lasty = y1;
	}

	if (z1>z2)
	{
		xz = (float)(z1-z2)/(float)(x2-x1);
		lastz = z2;
	} else
	{
		xz = (float)(z2-z1)/(float)(x2-x1);
		lastz = z1;
	}



	// For each step of x, y increments by:
	for (x = x1; x<=x2;++x)
	{
		y = (xy*(x-x1))+y1;
		z = (xz*(x-x1))+z1;
		setvoxel(x,y,z);
	}
	
}

// Delay loop.
// This is not calibrated to milliseconds,
// but we had allready made to many effects using this
// calibration when we figured it might be a good idea
// to calibrate it.
void delay_ms(uint16_t x)
{
  uint_fast8_t y, z;
  for ( ; x > 0 ; --x){
    for ( y = 0 ; y < 90 ; ++y){
      for ( z = 0 ; z < 6 ; ++z){
        asm volatile ("nop");
      }
    }
  }
}



// Shift the entire contents of the cube along an axis
// This is great for effects where you want to draw something
// on one side of the cube and have it flow towards the other
// side. Like rain flowing down the Z axiz.
void shift (char axis, int direction)
{
	int i, x ,y;
	int ii, iii;
	int state;

	for (i = 0; i < 8; ++i)
	{
		if (direction == -1)
		{
			ii = i;
		} else
		{
			ii = (7-i);
		}	
	
	
		for (x = 0; x < 8; ++x)
		{
			for (y = 0; y < 8; ++y)
			{
				if (direction == -1)
				{
					iii = ii+1;
				} else
				{
					iii = ii-1;
				}
				
				if (axis == AXIS_Z)
				{
					state = getvoxel(x,y,iii);
					altervoxel(x,y,ii,state);
				}
				
				if (axis == AXIS_Y)
				{
					state = getvoxel(x,iii,y);
					altervoxel(x,ii,y,state);
				}
				
				if (axis == AXIS_X)
				{
					state = getvoxel(iii,y,x);
					altervoxel(ii,y,x,state);
				}
			}
		}
	}
	
	if (direction == -1)
	{
		i = 7;
	} else
	{
		i = 0;
	}	
	
	for (x = 0; x < 8; ++x)
	{
		for (y = 0; y < 8; ++y)
		{
			if (axis == AXIS_Z)
				clrvoxel(x,y,i);
				
			if (axis == AXIS_Y)
				clrvoxel(x,i,y);
			
			if (axis == AXIS_X)
				clrvoxel(i,y,x);
		}
	}
}

// ********************************************************
const char font_data[128][8] PROGMEM = {
{ 0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00 },    // 0 :
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
 
  { 0x00,  0x3E,  0x41,  0x55,  0x41,  0x55,  0x49,  0x3E },    // 1 : 
                //              |          |
                //              |   *****  |
                //              |  *     * |
                //              |  * * * * |
                //              |  *     * |
                //              |  * * * * |
                //              |  *  *  * |
                //              |   *****  |
 
  { 0x00,  0x3E,  0x7F,  0x6B,  0x7F,  0x6B,  0x77,  0x3E },    // 2 : 
                //              |          |
                //              |   *****  |
                //              |  ******* |
                //              |  ** * ** |
                //              |  ******* |
                //              |  ** * ** |
                //              |  *** *** |
                //              |   *****  |
 
  { 0x00,  0x22,  0x77,  0x7F,  0x7F,  0x3E,  0x1C,  0x08 },    // 3 : 
                //              |          |
                //              |   *   *  |
                //              |  *** *** |
                //              |  ******* |
                //              |  ******* |
                //              |   *****  |
                //              |    ***   |
                //              |     *    |
 
  { 0x00,  0x08,  0x1C,  0x3E,  0x7F,  0x3E,  0x1C,  0x08 },    // 4 : 
                //              |          |
                //              |     *    |
                //              |    ***   |
                //              |   *****  |
                //              |  ******* |
                //              |   *****  |
                //              |    ***   |
                //              |     *    |
 
  { 0x00,  0x08,  0x1C,  0x2A,  0x7F,  0x2A,  0x08,  0x1C },    // 5 : 
                //              |          |
                //              |     *    |
                //              |    ***   |
                //              |   * * *  |
                //              |  ******* |
                //              |   * * *  |
                //              |     *    |
                //              |    ***   |
 
  { 0x00,  0x08,  0x1C,  0x3E,  0x7F,  0x3E,  0x08,  0x1C },    // 6 : 
                //              |          |
                //              |     *    |
                //              |    ***   |
                //              |   *****  |
                //              |  ******* |
                //              |   *****  |
                //              |     *    |
                //              |    ***   |
 
  { 0x00,  0x00,  0x1C,  0x3E,  0x3E,  0x3E,  0x1C,  0x00 },    // 7 :  
                //              |          |
                //              |          |
                //              |    ***   |
                //              |   *****  |
                //              |   *****  |
                //              |   *****  |
                //              |    ***   |
                //              |          |
 
  { 0xFF,  0xFF,  0xE3,  0xC1,  0xC1,  0xC1,  0xE3,  0xFF },    // 8 : 
                //              | ******** |
                //              | ******** |
                //              | ***   ** |
                //              | **     * |
                //              | **     * |
                //              | **     * |
                //              | ***   ** |
                //              | ******** |
 
  { 0x00,  0x00,  0x1C,  0x22,  0x22,  0x22,  0x1C,  0x00 },    // 9 :  
                //              |          |
                //              |          |
                //              |    ***   |
                //              |   *   *  |
                //              |   *   *  |
                //              |   *   *  |
                //              |    ***   |
                //              |          |
 
  { 0xFF,  0xFF,  0xE3,  0xDD,  0xDD,  0xDD,  0xE3,  0xFF },    // 10 :
 
                //              | ******** |
                //              | ******** |
                //              | ***   ** |
                //              | ** *** * |
                //              | ** *** * |
                //              | ** *** * |
                //              | ***   ** |
                //              | ******** |
 
  { 0x00,  0x0F,  0x03,  0x05,  0x39,  0x48,  0x48,  0x30 },    // 11 : 

                //              |          |
                //              |     **** |
                //              |       ** |
                //              |      * * |
                //              |   ***  * |
                //              |  *  *    |
                //              |  *  *    |
                //              |   **     |
 
  { 0x00,  0x08,  0x3E,  0x08,  0x1C,  0x22,  0x22,  0x1C },    // 12 : 
                //              |          |
                //              |     *    |
                //              |   *****  |
                //              |     *    |
                //              |    ***   |
                //              |   *   *  |
                //              |   *   *  |
                //              |    ***   |
 
  { 0x00,  0x18,  0x14,  0x10,  0x10,  0x30,  0x70,  0x60 },    // 13 :
 
                //              |          |
                //              |    **    |
                //              |    * *   |
                //              |    *     |
                //              |    *     |
                //              |   **     |
                //              |  ***     |
                //              |  **      |
 
  { 0x00,  0x0F,  0x19,  0x11,  0x13,  0x37,  0x76,  0x60 },    // 14 : 
                //              |          |
                //              |     **** |
                //              |    **  * |
                //              |    *   * |
                //              |    *  ** |
                //              |   ** *** |
                //              |  *** **  |
                //              |  **      |
 
  { 0x00,  0x08,  0x2A,  0x1C,  0x77,  0x1C,  0x2A,  0x08 },    // 15 : 
                //              |          |
                //              |     *    |
                //              |   * * *  |
                //              |    ***   |
                //              |  *** *** |
                //              |    ***   |
                //              |   * * *  |
                //              |     *    |
 
  { 0x00,  0x60,  0x78,  0x7E,  0x7F,  0x7E,  0x78,  0x60 },    // 16 : 
                //              |          |
                //              |  **      |
                //              |  ****    |
                //              |  ******  |
                //              |  ******* |
                //              |  ******  |
                //              |  ****    |
                //              |  **      |
 
  { 0x00,  0x03,  0x0F,  0x3F,  0x7F,  0x3F,  0x0F,  0x03 },    // 17 : 
                //              |          |
                //              |       ** |
                //              |     **** |
                //              |   ****** |
                //              |  ******* |
                //              |   ****** |
                //              |     **** |
                //              |       ** |
 
  { 0x00,  0x08,  0x1C,  0x2A,  0x08,  0x2A,  0x1C,  0x08 },    // 18 : 
                //              |          |
                //              |     *    |
                //              |    ***   |
                //              |   * * *  |
                //              |     *    |
                //              |   * * *  |
                //              |    ***   |
                //              |     *    |
 
  { 0x00,  0x66,  0x66,  0x66,  0x66,  0x00,  0x66,  0x66 },    // 19 : 
                //              |          |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |          |
                //              |  **  **  |
                //              |  **  **  |
 
  { 0x00,  0x3F,  0x65,  0x65,  0x3D,  0x05,  0x05,  0x05 },    // 20 : 
                //              |          |
                //              |   ****** |
                //              |  **  * * |
                //              |  **  * * |
                //              |   **** * |
                //              |      * * |
                //              |      * * |
                //              |      * * |
 
  { 0x00,  0x0C,  0x32,  0x48,  0x24,  0x12,  0x4C,  0x30 },    // 21 : 
                //              |          |
                //              |     **   |
                //              |   **  *  |
                //              |  *  *    |
                //              |   *  *   |
                //              |    *  *  |
                //              |  *  **   |
                //              |   **     |
 
  { 0x00,  0x00,  0x00,  0x00,  0x00,  0x7F,  0x7F,  0x7F },    // 22 : 
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |  ******* |
                //              |  ******* |
                //              |  ******* |
 
  { 0x00,  0x08,  0x1C,  0x2A,  0x08,  0x2A,  0x1C,  0x3E },    // 23 : 
                //              |          |
                //              |     *    |
                //              |    ***   |
                //              |   * * *  |
                //              |     *    |
                //              |   * * *  |
                //              |    ***   |
                //              |   *****  |
 
  { 0x00,  0x08,  0x1C,  0x3E,  0x7F,  0x1C,  0x1C,  0x1C },    // 24 : 
                //              |          |
                //              |     *    |
                //              |    ***   |
                //              |   *****  |
                //              |  ******* |
                //              |    ***   |
                //              |    ***   |
                //              |    ***   |
 
  { 0x00,  0x1C,  0x1C,  0x1C,  0x7F,  0x3E,  0x1C,  0x08 },    // 25 : 
                //              |          |
                //              |    ***   |
                //              |    ***   |
                //              |    ***   |
                //              |  ******* |
                //              |   *****  |
                //              |    ***   |
                //              |     *    |
 
  { 0x00,  0x08,  0x0C,  0x7E,  0x7F,  0x7E,  0x0C,  0x08 },    // 26 : 
                //              |          |
                //              |     *    |
                //              |     **   |
                //              |  ******  |
                //              |  ******* |
                //              |  ******  |
                //              |     **   |
                //              |     *    |
 
  { 0x00,  0x08,  0x18,  0x3F,  0x7F,  0x3F,  0x18,  0x08 },    // 27 : 
                //              |          |
                //              |     *    |
                //              |    **    |
                //              |   ****** |
                //              |  ******* |
                //              |   ****** |
                //              |    **    |
                //              |     *    |
 
  { 0x00,  0x00,  0x00,  0x70,  0x70,  0x70,  0x7F,  0x7F },    // 28 : 
                //              |          |
                //              |          |
                //              |          |
                //              |  ***     |
                //              |  ***     |
                //              |  ***     |
                //              |  ******* |
                //              |  ******* |
 
  { 0x00,  0x00,  0x14,  0x22,  0x7F,  0x22,  0x14,  0x00 },    // 29 : 
                //              |          |
                //              |          |
                //              |    * *   |
                //              |   *   *  |
                //              |  ******* |
                //              |   *   *  |
                //              |    * *   |
                //              |          |
 
  { 0x00,  0x08,  0x1C,  0x1C,  0x3E,  0x3E,  0x7F,  0x7F },    // 30 : 
                //              |          |
                //              |     *    |
                //              |    ***   |
                //              |    ***   |
                //              |   *****  |
                //              |   *****  |
                //              |  ******* |
                //              |  ******* |
 
  { 0x00,  0x7F,  0x7F,  0x3E,  0x3E,  0x1C,  0x1C,  0x08 },    // 31 : 
                //              |          |
                //              |  ******* |
                //              |  ******* |
                //              |   *****  |
                //              |   *****  |
                //              |    ***   |
                //              |    ***   |
                //              |     *    |
 
  { 0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00 },    // 32 :  
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
 
  { 0x00,  0x18,  0x18,  0x18,  0x18,  0x18,  0x00,  0x18 },    // 33 : !
                //              |          |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |          |
                //              |    **    |
 
  { 0x00,  0x36,  0x36,  0x14,  0x00,  0x00,  0x00,  0x00 },    // 34 : "
                //              |          |
                //              |   ** **  |
                //              |   ** **  |
                //              |    * *   |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
 
  { 0x00,  0x36,  0x36,  0x7F,  0x36,  0x7F,  0x36,  0x36 },    // 35 : #
                //              |          |
                //              |   ** **  |
                //              |   ** **  |
                //              |  ******* |
                //              |   ** **  |
                //              |  ******* |
                //              |   ** **  |
                //              |   ** **  |
 
  { 0x00,  0x08,  0x1E,  0x20,  0x1C,  0x02,  0x3C,  0x08 },    // 36 : $
                //              |          |
                //              |     *    |
                //              |    ****  |
                //              |   *      |
                //              |    ***   |
                //              |       *  |
                //              |   ****   |
                //              |     *    |
 
  { 0x00,  0x60,  0x66,  0x0C,  0x18,  0x30,  0x66,  0x06 },    // 37 : %
                //              |          |
                //              |  **      |
                //              |  **  **  |
                //              |     **   |
                //              |    **    |
                //              |   **     |
                //              |  **  **  |
                //              |      **  |
 
  { 0x00,  0x3C,  0x66,  0x3C,  0x28,  0x65,  0x66,  0x3F },    // 38 : &
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |   ****   |
                //              |   * *    |
                //              |  **  * * |
                //              |  **  **  |
                //              |   ****** |
 
  { 0x00,  0x18,  0x18,  0x18,  0x30,  0x00,  0x00,  0x00 },    // 39 : '
                //              |          |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |   **     |
                //              |          |
                //              |          |
                //              |          |
 
  { 0x00,  0x60,  0x30,  0x18,  0x18,  0x18,  0x30,  0x60 },    // 40 : (
                //              |          |
                //              |  **      |
                //              |   **     |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |   **     |
                //              |  **      |
 
  { 0x00,  0x06,  0x0C,  0x18,  0x18,  0x18,  0x0C,  0x06 },    // 41 : )
                //              |          |
                //              |      **  |
                //              |     **   |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |     **   |
                //              |      **  |
 
  { 0x00,  0x00,  0x36,  0x1C,  0x7F,  0x1C,  0x36,  0x00 },    // 42 : *
                //              |          |
                //              |          |
                //              |   ** **  |
                //              |    ***   |
                //              |  ******* |
                //              |    ***   |
                //              |   ** **  |
                //              |          |
 
  { 0x00,  0x00,  0x08,  0x08,  0x3E,  0x08,  0x08,  0x00 },    // 43 : +
                //              |          |
                //              |          |
                //              |     *    |
                //              |     *    |
                //              |   *****  |
                //              |     *    |
                //              |     *    |
                //              |          |
 
  { 0x00,  0x00,  0x00,  0x00,  0x30,  0x30,  0x30,  0x60 },    // 44 : ,
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |   **     |
                //              |   **     |
                //              |   **     |
                //              |  **      |
 
  { 0x00,  0x00,  0x00,  0x00,  0x3C,  0x00,  0x00,  0x00 },    // 45 : -
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |   ****   |
                //              |          |
                //              |          |
                //              |          |
 
  { 0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x60,  0x60 },    // 46 : .
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |  **      |
                //              |  **      |
 
  { 0x00,  0x00,  0x06,  0x0C,  0x18,  0x30,  0x60,  0x00 },    // 47 : /
                //              |          |
                //              |          |
                //              |      **  |
                //              |     **   |
                //              |    **    |
                //              |   **     |
                //              |  **      |
                //              |          |
 
  { 0x00,  0x3C,  0x66,  0x6E,  0x76,  0x66,  0x66,  0x3C },    // 48 : 0
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  ** ***  |
                //              |  *** **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |   ****   |
 
  { 0x00,  0x18,  0x18,  0x38,  0x18,  0x18,  0x18,  0x7E },    // 49 : 1
                //              |          |
                //              |    **    |
                //              |    **    |
                //              |   ***    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |  ******  |
 
  { 0x00,  0x3C,  0x66,  0x06,  0x0C,  0x30,  0x60,  0x7E },    // 50 : 2
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |      **  |
                //              |     **   |
                //              |   **     |
                //              |  **      |
                //              |  ******  |
 
  { 0x00,  0x3C,  0x66,  0x06,  0x1C,  0x06,  0x66,  0x3C },    // 51 : 3
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |      **  |
                //              |    ***   |
                //              |      **  |
                //              |  **  **  |
                //              |   ****   |
 
  { 0x00,  0x0C,  0x1C,  0x2C,  0x4C,  0x7E,  0x0C,  0x0C },    // 52 : 4
                //              |          |
                //              |     **   |
                //              |    ***   |
                //              |   * **   |
                //              |  *  **   |
                //              |  ******  |
                //              |     **   |
                //              |     **   |
 
  { 0x00,  0x7E,  0x60,  0x7C,  0x06,  0x06,  0x66,  0x3C },    // 53 : 5
                //              |          |
                //              |  ******  |
                //              |  **      |
                //              |  *****   |
                //              |      **  |
                //              |      **  |
                //              |  **  **  |
                //              |   ****   |
 
  { 0x00,  0x3C,  0x66,  0x60,  0x7C,  0x66,  0x66,  0x3C },    // 54 : 6
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  **      |
                //              |  *****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |   ****   |
 
  { 0x00,  0x7E,  0x66,  0x0C,  0x0C,  0x18,  0x18,  0x18 },    // 55 : 7
                //              |          |
                //              |  ******  |
                //              |  **  **  |
                //              |     **   |
                //              |     **   |
                //              |    **    |
                //              |    **    |
                //              |    **    |
 
  { 0x00,  0x3C,  0x66,  0x66,  0x3C,  0x66,  0x66,  0x3C },    // 56 : 8
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |   ****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |   ****   |
 
  { 0x00,  0x3C,  0x66,  0x66,  0x3E,  0x06,  0x66,  0x3C },    // 57 : 9
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |   *****  |
                //              |      **  |
                //              |  **  **  |
                //              |   ****   |
 
  { 0x00,  0x00,  0x18,  0x18,  0x00,  0x18,  0x18,  0x00 },    // 58 : :
                //              |          |
                //              |          |
                //              |    **    |
                //              |    **    |
                //              |          |
                //              |    **    |
                //              |    **    |
                //              |          |
 
  { 0x00,  0x00,  0x18,  0x18,  0x00,  0x18,  0x18,  0x30 },    // 59 : ;
                //              |          |
                //              |          |
                //              |    **    |
                //              |    **    |
                //              |          |
                //              |    **    |
                //              |    **    |
                //              |   **     |
 
  { 0x00,  0x06,  0x0C,  0x18,  0x30,  0x18,  0x0C,  0x06 },    // 60 : <
                //              |          |
                //              |      **  |
                //              |     **   |
                //              |    **    |
                //              |   **     |
                //              |    **    |
                //              |     **   |
                //              |      **  |
 
  { 0x00,  0x00,  0x00,  0x3C,  0x00,  0x3C,  0x00,  0x00 },    // 61 : =
                //              |          |
                //              |          |
                //              |          |
                //              |   ****   |
                //              |          |
                //              |   ****   |
                //              |          |
                //              |          |
 
  { 0x00,  0x60,  0x30,  0x18,  0x0C,  0x18,  0x30,  0x60 },    // 62 : >
                //              |          |
                //              |  **      |
                //              |   **     |
                //              |    **    |
                //              |     **   |
                //              |    **    |
                //              |   **     |
                //              |  **      |
 
  { 0x00,  0x3C,  0x66,  0x06,  0x1C,  0x18,  0x00,  0x18 },    // 63 : ?
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |      **  |
                //              |    ***   |
                //              |    **    |
                //              |          |
                //              |    **    |
 
  { 0x00,  0x38,  0x44,  0x5C,  0x58,  0x42,  0x3C,  0x00 },    // 64 : @
                //              |          |
                //              |   ***    |
                //              |  *   *   |
                //              |  * ***   |
                //              |  * **    |
                //              |  *    *  |
                //              |   ****   |
                //              |          |
 
  { 0x00,  0x3C,  0x66,  0x66,  0x7E,  0x66,  0x66,  0x66 },    // 65 : A
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  ******  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
 
  { 0x00,  0x7C,  0x66,  0x66,  0x7C,  0x66,  0x66,  0x7C },    // 66 : B
                //              |          |
                //              |  *****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  *****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  *****   |
 
  { 0x00,  0x3C,  0x66,  0x60,  0x60,  0x60,  0x66,  0x3C },    // 67 : C
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  **      |
                //              |  **      |
                //              |  **      |
                //              |  **  **  |
                //              |   ****   |
 
  { 0x00,  0x7C,  0x66,  0x66,  0x66,  0x66,  0x66,  0x7C },    // 68 : D
                //              |          |
                //              |  *****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  *****   |
 
  { 0x00,  0x7E,  0x60,  0x60,  0x7C,  0x60,  0x60,  0x7E },    // 69 : E
                //              |          |
                //              |  ******  |
                //              |  **      |
                //              |  **      |
                //              |  *****   |
                //              |  **      |
                //              |  **      |
                //              |  ******  |
 
  { 0x00,  0x7E,  0x60,  0x60,  0x7C,  0x60,  0x60,  0x60 },    // 70 : F
                //              |          |
                //              |  ******  |
                //              |  **      |
                //              |  **      |
                //              |  *****   |
                //              |  **      |
                //              |  **      |
                //              |  **      |
 
  { 0x00,  0x3C,  0x66,  0x60,  0x60,  0x6E,  0x66,  0x3C },    // 71 : G
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  **      |
                //              |  **      |
                //              |  ** ***  |
                //              |  **  **  |
                //              |   ****   |
 
  { 0x00,  0x66,  0x66,  0x66,  0x7E,  0x66,  0x66,  0x66 },    // 72 : H
                //              |          |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  ******  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
 
  { 0x00,  0x3C,  0x18,  0x18,  0x18,  0x18,  0x18,  0x3C },    // 73 : I
                //              |          |
                //              |   ****   |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |   ****   |
 
  { 0x00,  0x1E,  0x0C,  0x0C,  0x0C,  0x6C,  0x6C,  0x38 },    // 74 : J
                //              |          |
                //              |    ****  |
                //              |     **   |
                //              |     **   |
                //              |     **   |
                //              |  ** **   |
                //              |  ** **   |
                //              |   ***    |
 
  { 0x00,  0x66,  0x6C,  0x78,  0x70,  0x78,  0x6C,  0x66 },    // 75 : K
                //              |          |
                //              |  **  **  |
                //              |  ** **   |
                //              |  ****    |
                //              |  ***     |
                //              |  ****    |
                //              |  ** **   |
                //              |  **  **  |
 
  { 0x00,  0x60,  0x60,  0x60,  0x60,  0x60,  0x60,  0x7E },    // 76 : L
                //              |          |
                //              |  **      |
                //              |  **      |
                //              |  **      |
                //              |  **      |
                //              |  **      |
                //              |  **      |
                //              |  ******  |
 
  { 0x00,  0x63,  0x77,  0x7F,  0x6B,  0x63,  0x63,  0x63 },    // 77 : M
                //              |          |
                //              |  **   ** |
                //              |  *** *** |
                //              |  ******* |
                //              |  ** * ** |
                //              |  **   ** |
                //              |  **   ** |
                //              |  **   ** |
 
  { 0x00,  0x63,  0x73,  0x7B,  0x6F,  0x67,  0x63,  0x63 },    // 78 : N
                //              |          |
                //              |  **   ** |
                //              |  ***  ** |
                //              |  **** ** |
                //              |  ** **** |
                //              |  **  *** |
                //              |  **   ** |
                //              |  **   ** |
 
  { 0x00,  0x3C,  0x66,  0x66,  0x66,  0x66,  0x66,  0x3C },    // 79 : O
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |   ****   |
 
  { 0x00,  0x7C,  0x66,  0x66,  0x66,  0x7C,  0x60,  0x60 },    // 80 : P
                //              |          |
                //              |  *****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  *****   |
                //              |  **      |
                //              |  **      |
 
  { 0x00,  0x3C,  0x66,  0x66,  0x66,  0x6E,  0x3C,  0x06 },    // 81 : Q
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  ** ***  |
                //              |   ****   |
                //              |      **  |
 
  { 0x00,  0x7C,  0x66,  0x66,  0x7C,  0x78,  0x6C,  0x66 },    // 82 : R
                //              |          |
                //              |  *****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  *****   |
                //              |  ****    |
                //              |  ** **   |
                //              |  **  **  |
 
  { 0x00,  0x3C,  0x66,  0x60,  0x3C,  0x06,  0x66,  0x3C },    // 83 : S
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  **      |
                //              |   ****   |
                //              |      **  |
                //              |  **  **  |
                //              |   ****   |
 
  { 0x00,  0x7E,  0x5A,  0x18,  0x18,  0x18,  0x18,  0x18 },    // 84 : T
                //              |          |
                //              |  ******  |
                //              |  * ** *  |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
 
  { 0x00,  0x66,  0x66,  0x66,  0x66,  0x66,  0x66,  0x3E },    // 85 : U
                //              |          |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |   *****  |
 
  { 0x00,  0x66,  0x66,  0x66,  0x66,  0x66,  0x3C,  0x18 },    // 86 : V
                //              |          |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |   ****   |
                //              |    **    |
 
  { 0x00,  0x63,  0x63,  0x63,  0x6B,  0x7F,  0x77,  0x63 },    // 87 : W
                //              |          |
                //              |  **   ** |
                //              |  **   ** |
                //              |  **   ** |
                //              |  ** * ** |
                //              |  ******* |
                //              |  *** *** |
                //              |  **   ** |
 
  { 0x00,  0x63,  0x63,  0x36,  0x1C,  0x36,  0x63,  0x63 },    // 88 : X
                //              |          |
                //              |  **   ** |
                //              |  **   ** |
                //              |   ** **  |
                //              |    ***   |
                //              |   ** **  |
                //              |  **   ** |
                //              |  **   ** |
 
  { 0x00,  0x66,  0x66,  0x66,  0x3C,  0x18,  0x18,  0x18 },    // 89 : Y
                //              |          |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |   ****   |
                //              |    **    |
                //              |    **    |
                //              |    **    |
 
  { 0x00,  0x7E,  0x06,  0x0C,  0x18,  0x30,  0x60,  0x7E },    // 90 : Z
                //              |          |
                //              |  ******  |
                //              |      **  |
                //              |     **   |
                //              |    **    |
                //              |   **     |
                //              |  **      |
                //              |  ******  |
 
  { 0x00,  0x1E,  0x18,  0x18,  0x18,  0x18,  0x18,  0x1E },    // 91 : [
                //              |          |
                //              |    ****  |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    ****  |
 
  { 0x00,  0x00,  0x60,  0x30,  0x18,  0x0C,  0x06,  0x00 },    // 92 : \
                //              |          |
                //              |          |
                //              |  **      |
                //              |   **     |
                //              |    **    |
                //              |     **   |
                //              |      **  |
                //              |          |
 
  { 0x00,  0x78,  0x18,  0x18,  0x18,  0x18,  0x18,  0x78 },    // 93 : ]
                //              |          |
                //              |  ****    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |  ****    |
 
  { 0x00,  0x08,  0x14,  0x22,  0x41,  0x00,  0x00,  0x00 },    // 94 : ^
                //              |          |
                //              |     *    |
                //              |    * *   |
                //              |   *   *  |
                //              |  *     * |
                //              |          |
                //              |          |
                //              |          |
 
  { 0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x7F },    // 95 : _
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |  ******* |
 
  { 0x00,  0x0C,  0x0C,  0x06,  0x00,  0x00,  0x00,  0x00 },    // 96 : `
                //              |          |
                //              |     **   |
                //              |     **   |
                //              |      **  |
                //              |          |
                //              |          |
                //              |          |
                //              |          |
 
  { 0x00,  0x00,  0x00,  0x3C,  0x06,  0x3E,  0x66,  0x3E },    // 97 : a
                //              |          |
                //              |          |
                //              |          |
                //              |   ****   |
                //              |      **  |
                //              |   *****  |
                //              |  **  **  |
                //              |   *****  |
 
  { 0x00,  0x60,  0x60,  0x60,  0x7C,  0x66,  0x66,  0x7C },    // 98 : b
                //              |          |
                //              |  **      |
                //              |  **      |
                //              |  **      |
                //              |  *****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  *****   |
 
  { 0x00,  0x00,  0x00,  0x3C,  0x66,  0x60,  0x66,  0x3C },    // 99 : c
                //              |          |
                //              |          |
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  **      |
                //              |  **  **  |
                //              |   ****   |
 
  { 0x00,  0x06,  0x06,  0x06,  0x3E,  0x66,  0x66,  0x3E },    // 100 : d
                //              |          |
                //              |      **  |
                //              |      **  |
                //              |      **  |
                //              |   *****  |
                //              |  **  **  |
                //              |  **  **  |
                //              |   *****  |
 
  { 0x00,  0x00,  0x00,  0x3C,  0x66,  0x7E,  0x60,  0x3C },    // 101 : e
                //              |          |
                //              |          |
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  ******  |
                //              |  **      |
                //              |   ****   |
 
  { 0x00,  0x1C,  0x36,  0x30,  0x30,  0x7C,  0x30,  0x30 },    // 102 : f
                //              |          |
                //              |    ***   |
                //              |   ** **  |
                //              |   **     |
                //              |   **     |
                //              |  *****   |
                //              |   **     |
                //              |   **     |
 
  { 0x00,  0x00,  0x3E,  0x66,  0x66,  0x3E,  0x06,  0x3C },    // 103 : g
                //              |          |
                //              |          |
                //              |   *****  |
                //              |  **  **  |
                //              |  **  **  |
                //              |   *****  |
                //              |      **  |
                //              |   ****   |
 
  { 0x00,  0x60,  0x60,  0x60,  0x7C,  0x66,  0x66,  0x66 },    // 104 : h
                //              |          |
                //              |  **      |
                //              |  **      |
                //              |  **      |
                //              |  *****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
 
  { 0x00,  0x00,  0x18,  0x00,  0x18,  0x18,  0x18,  0x3C },    // 105 : i
                //              |          |
                //              |          |
                //              |    **    |
                //              |          |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |   ****   |
 
  { 0x00,  0x0C,  0x00,  0x0C,  0x0C,  0x6C,  0x6C,  0x38 },    // 106 : j
                //              |          |
                //              |     **   |
                //              |          |
                //              |     **   |
                //              |     **   |
                //              |  ** **   |
                //              |  ** **   |
                //              |   ***    |
 
  { 0x00,  0x60,  0x60,  0x66,  0x6C,  0x78,  0x6C,  0x66 },    // 107 : k
                //              |          |
                //              |  **      |
                //              |  **      |
                //              |  **  **  |
                //              |  ** **   |
                //              |  ****    |
                //              |  ** **   |
                //              |  **  **  |
 
  { 0x00,  0x18,  0x18,  0x18,  0x18,  0x18,  0x18,  0x18 },    // 108 : l
                //              |          |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |    **    |
 
  { 0x00,  0x00,  0x00,  0x63,  0x77,  0x7F,  0x6B,  0x6B },    // 109 : m
                //              |          |
                //              |          |
                //              |          |
                //              |  **   ** |
                //              |  *** *** |
                //              |  ******* |
                //              |  ** * ** |
                //              |  ** * ** |
 
  { 0x00,  0x00,  0x00,  0x7C,  0x7E,  0x66,  0x66,  0x66 },    // 110 : n
                //              |          |
                //              |          |
                //              |          |
                //              |  *****   |
                //              |  ******  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
 
  { 0x00,  0x00,  0x00,  0x3C,  0x66,  0x66,  0x66,  0x3C },    // 111 : o
                //              |          |
                //              |          |
                //              |          |
                //              |   ****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |   ****   |
 
  { 0x00,  0x00,  0x7C,  0x66,  0x66,  0x7C,  0x60,  0x60 },    // 112 : p
                //              |          |
                //              |          |
                //              |  *****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  *****   |
                //              |  **      |
                //              |  **      |
 
  { 0x00,  0x00,  0x3C,  0x6C,  0x6C,  0x3C,  0x0D,  0x0F },    // 113 : q
                //              |          |
                //              |          |
                //              |   ****   |
                //              |  ** **   |
                //              |  ** **   |
                //              |   ****   |
                //              |     ** * |
                //              |     **** |
 
  { 0x00,  0x00,  0x00,  0x7C,  0x66,  0x66,  0x60,  0x60 },    // 114 : r
                //              |          |
                //              |          |
                //              |          |
                //              |  *****   |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **      |
                //              |  **      |
 
  { 0x00,  0x00,  0x00,  0x3E,  0x40,  0x3C,  0x02,  0x7C },    // 115 : s
                //              |          |
                //              |          |
                //              |          |
                //              |   *****  |
                //              |  *       |
                //              |   ****   |
                //              |       *  |
                //              |  *****   |
 
  { 0x00,  0x00,  0x18,  0x18,  0x7E,  0x18,  0x18,  0x18 },    // 116 : t
                //              |          |
                //              |          |
                //              |    **    |
                //              |    **    |
                //              |  ******  |
                //              |    **    |
                //              |    **    |
                //              |    **    |
 
  { 0x00,  0x00,  0x00,  0x66,  0x66,  0x66,  0x66,  0x3E },    // 117 : u
                //              |          |
                //              |          |
                //              |          |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |  **  **  |
                //              |   *****  |
 
  { 0x00,  0x00,  0x00,  0x00,  0x66,  0x66,  0x3C,  0x18 },    // 118 : v
                //              |          |
                //              |          |
                //              |          |
                //              |          |
                //              |  **  **  |
                //              |  **  **  |
                //              |   ****   |
                //              |    **    |
 
  { 0x00,  0x00,  0x00,  0x63,  0x6B,  0x6B,  0x6B,  0x3E },    // 119 : w
                //              |          |
                //              |          |
                //              |          |
                //              |  **   ** |
                //              |  ** * ** |
                //              |  ** * ** |
                //              |  ** * ** |
                //              |   *****  |
 
  { 0x00,  0x00,  0x00,  0x66,  0x3C,  0x18,  0x3C,  0x66 },    // 120 : x
                //              |          |
                //              |          |
                //              |          |
                //              |  **  **  |
                //              |   ****   |
                //              |    **    |
                //              |   ****   |
                //              |  **  **  |
 
  { 0x00,  0x00,  0x00,  0x66,  0x66,  0x3E,  0x06,  0x3C },    // 121 : y
                //              |          |
                //              |          |
                //              |          |
                //              |  **  **  |
                //              |  **  **  |
                //              |   *****  |
                //              |      **  |
                //              |   ****   |
 
  { 0x00,  0x00,  0x00,  0x3C,  0x0C,  0x18,  0x30,  0x3C },    // 122 : z
                //              |          |
                //              |          |
                //              |          |
                //              |   ****   |
                //              |     **   |
                //              |    **    |
                //              |   **     |
                //              |   ****   |
 
  { 0x00,  0x0E,  0x18,  0x18,  0x30,  0x18,  0x18,  0x0E },    // 123 : {
                //              |          |
                //              |     ***  |
                //              |    **    |
                //              |    **    |
                //              |   **     |
                //              |    **    |
                //              |    **    |
                //              |     ***  |
 
  { 0x00,  0x18,  0x18,  0x18,  0x00,  0x18,  0x18,  0x18 },    // 124 : |
                //              |          |
                //              |    **    |
                //              |    **    |
                //              |    **    |
                //              |          |
                //              |    **    |
                //              |    **    |
                //              |    **    |
 
  { 0x00,  0x70,  0x18,  0x18,  0x0C,  0x18,  0x18,  0x70 },    // 125 : }
                //              |          |
                //              |  ***     |
                //              |    **    |
                //              |    **    |
                //              |     **   |
                //              |    **    |
                //              |    **    |
                //              |  ***     |
 
  { 0x00,  0x00,  0x00,  0x3A,  0x6C,  0x00,  0x00,  0x00 },    // 126 : ~
                //              |          |
                //              |          |
                //              |          |
                //              |   *** *  |
                //              |  ** **   |
                //              |          |
                //              |          |
                //              |          |
 
  { 0x00,  0x08,  0x1C,  0x36,  0x63,  0x41,  0x41,  0x7F }    // 127 : 
                //              |          |
                //              |     *    |
                //              |    ***   |
                //              |   ** **  |
                //              |  **   ** |
                //              |  *     * |
                //              |  *     * |
                //              |  ******* |
 
 
};
 
void effect_text(int delayt){
  fill(0x00);
  for (int ltr = 0; ltr < charNum; ++ltr){// For each letter in string array
    for(int dist = 0; dist < 8; ++dist) { //bring letter forward
      fill(0x00);//blank cube
      int rev = 0;
      for (int rw = 7; rw >= 0; --rw) {//copy rows
         // put this in for normal cube: cube[rev][dist] = bitswap(font_data[string[ltr]][rw]);
//         cube[rev][dist] = font_data[string[ltr]][rw];
         cube[rev][dist] = pgm_read_byte(&font_data[string[ltr]][rw]);
        // cube[dist][rev] = font_data[string[ltr]][rw];
         // use above line for backward ass cubes
         ++rev;
      }
      delay_ms(delayt);
    }
  }
}
 
 
void effect_text_up (int delayt)
{
  fill(0x00);
  for (int ltr = 0; ltr < charNum; ++ltr){// For each letter in string array
    for(int dist = 0; dist < 8; ++dist) { //bring letter forward
      fill(0x00);//blank cube
      int rev = 0;
      for (int rw = 7; rw >= 0; --rw) {//copy rows
         // put this in for miswired backwards cube: cube[rev][dist] = bitswap(font_data[string[ltr]][rw]);
         // cube[rev][dist] = font_data[string[ltr]][rw];
        cube[dist][rev] = pgm_read_byte(&font_data[string[ltr]][rw]);
         // use above line for proper cubes
         ++rev;
      }
      delay_ms(delayt);
    }
  }
}

void int_ripples(int iterations, int delay)
{
	// 16 values for square root of a^2+b^2.  index a*4+b = 10*sqrt
	// This gives the distance to 3.5,3.5 from the point
	unsigned char sqrt_LUT[]={49,43,38,35,43,35,29,26,38,29,21,16,35,25,16,7};
	//LUT_START // Macro from new tottymath.  Commented and replaced with full code
	unsigned char LUT[65];
	init_LUT(LUT);
	int i;
	unsigned char x,y,height,distance;
	for (i=0;i<iterations*4;i+=4)
	{
		fill(0x00);
		for (x=0;x<4;++x)
			for(y=0;y<4;++y)
			{
				// x+y*4 gives no. from 0-15 for sqrt_LUT
				distance=sqrt_LUT[x+y*4];// distance is 0-50 roughly
				// height is sin of distance + iteration*4
				//height=4+totty_sin(LUT,distance+i)/52;
				height=(196+totty_sin(LUT,distance+i))/49;
				// Use 4-way mirroring to save on calculations
				setvoxel(x,y,height);
				setvoxel(7-x,y,height);
				setvoxel(x,7-y,height);
				setvoxel(7-x,7-y,height);
			}
		delay_ms(delay);
	}
}

void side_ripples(int iterations, int delay)
{
	// 16 values for square root of a^2+b^2.  index a*4+b = 10*sqrt
	// This gives the distance to 3.5,3.5 from the point
	unsigned char sqrt_LUT[]={49,43,38,35,43,35,29,26,38,29,21,16,35,25,16,7};
	//LUT_START // Macro from new tottymath.  Commented and replaced with full code
	unsigned char LUT[65];
	init_LUT(LUT);
	int i;
	unsigned char x,y,height,distance;
	for (i=0;i<iterations*4;i+=4)
	{
		fill(0x00);
		for (x=0;x<4;++x)
			for(y=0;y<4;++y)
			{
				// x+y*4 gives no. from 0-15 for sqrt_LUT
				distance=sqrt_LUT[x+y*4];// distance is 0-50 roughly
				// height is sin of distance + iteration*4
				//height=4+totty_sin(LUT,distance+i)/52;
				height=(196+totty_sin(LUT,distance+i))/49;
				// Use 4-way mirroring to save on calculations
				setvoxel(x,height,y);
				setvoxel(7-x,height,y);
				setvoxel(x,height,7-y);
				setvoxel(7-x,height,7-y);
				setvoxel(x,7-height,y);
				setvoxel(7-x,7-height,y);
				setvoxel(x,7-height,7-y);
				setvoxel(7-x,7-height,7-y);

			}
		delay_ms(delay);
	}
}


void mirror_ripples(int iterations, int delay)
{
	// 16 values for square root of a^2+b^2.  index a*4+b = 10*sqrt
	// This gives the distance to 3.5,3.5 from the point
	unsigned char sqrt_LUT[]={49,43,38,35,43,35,29,26,38,29,21,16,35,25,16,7};
	//LUT_START // Macro from new tottymath.  Commented and replaced with full code
	unsigned char LUT[65];
	init_LUT(LUT);
	int i;
	unsigned char x,y,height,distance;
	for (i=0;i<iterations*4;i+=4)
	{
		fill(0x00);
		for (x=0;x<4;++x)
			for(y=0;y<4;++y)
			{
				// x+y*4 gives no. from 0-15 for sqrt_LUT
				distance=sqrt_LUT[x+y*4];// distance is 0-50 roughly
				// height is sin of distance + iteration*4
				//height=4+totty_sin(LUT,distance+i)/52;
				height=(196+totty_sin(LUT,distance+i))/49;
				// Use 4-way mirroring to save on calculations
				setvoxel(x,y,height);
				setvoxel(7-x,y,height);
				setvoxel(x,7-y,height);
				setvoxel(7-x,7-y,height);
				setvoxel(x,y,7-height);
				setvoxel(7-x,y,7-height);
				setvoxel(x,7-y,7-height);
				setvoxel(7-x,7-y,7-height);

			}
		delay_ms(delay);
	}
}

//**************************************************
 
unsigned char bitswap (unsigned char x)//Reverses a byte (so letters aren't backwards);
{  byte result;
 
    asm("mov __tmp_reg__, %[in] \n\t"
      "lsl __tmp_reg__  \n\t"   /* shift out high bit to carry */
      "ror %[out] \n\t"  /* rotate carry __tmp_reg__to low bit (eventually) */
      "lsl __tmp_reg__  \n\t"   /* 2 */
      "ror %[out] \n\t"
      "lsl __tmp_reg__  \n\t"   /* 3 */
      "ror %[out] \n\t"
      "lsl __tmp_reg__  \n\t"   /* 4 */
      "ror %[out] \n\t"
     
      "lsl __tmp_reg__  \n\t"   /* 5 */
      "ror %[out] \n\t"
      "lsl __tmp_reg__  \n\t"   /* 6 */
      "ror %[out] \n\t"
      "lsl __tmp_reg__  \n\t"   /* 7 */
      "ror %[out] \n\t"
      "lsl __tmp_reg__  \n\t"   /* 8 */
      "ror %[out] \n\t"
      
      : [out] "=r" (result) : [in] "r" (x));
      return(result);
}
 
//**************************************************
  void zoom_pyramid_clear(){


  //1
  
  box_walls(0,0,0,7,0,7); 
  delay_ms(500);  
  
  //2

  //Pyramid
    box_wireframe(0,0,0,7,0,1);    
    
   clrplane_y(0);
  delay_ms(500);  
  
  //3

  //Pyramid  
     clrplane_y(1);
  box_walls(0,2,0,7,2,7); 
  delay_ms(500);

  //4

  //Pyramid
     clrplane_y(2);
  box_walls(0,3,0,7,3,7);  
  delay_ms(500);

  //5

  //Pyramid
     clrplane_y(3);
  box_walls(0,4,0,7,4,7); 
  delay_ms(500);

  //5

  //Pyramid
  
     clrplane_y(4);
  box_walls(0,5,0,7,5,7); 
  delay_ms(500);
  //6
  

  //Pyramid
  
     clrplane_y(5);
      box_walls(0,6,0,7,6,7); 
  delay_ms(500);
  //7

  //Pyramid
  
  clrplane_y(6);
  box_walls(0,7,0,7,7,7);     
  delay_ms(500);  
    

  clrplane_y(7);
  delay_ms(10000);
  
}

void zoom_pyramid(){
int i,j,k,time;

  //1
  fill(0x00);
  
  box_walls(0,0,0,7,0,7); 
  delay_ms(500);  
  
  //2
  fill(0x00);  
  //Pyramid
    box_wireframe(0,0,0,7,0,1);    
    
   box_walls(0,1,0,7,1,7);    
  delay_ms(500);  
  
  //3
  fill(0x00);
  //Pyramid  
    box_wireframe(0,0,0,7,1,1);
    box_wireframe(1,1,2,6,1,3);  

  box_walls(0,2,0,7,2,7); 
  delay_ms(500);

  //4
  fill(0x00);
  //Pyramid
    box_wireframe(0,0,0,7,2,1);
    box_wireframe(1,1,2,6,2,3);  
    box_wireframe(2,2,4,5,2,5);  

  box_walls(0,3,0,7,3,7);  
  delay_ms(500);

  //5
  fill(0x00);
  //Pyramid
    box_wireframe(0,0,0,7,3,1);
    box_wireframe(1,1,2,6,3,3);  
    box_wireframe(2,2,4,5,3,5);
    box_wireframe(3,3,6,4,3,7);

  box_walls(0,4,0,7,4,7); 
  delay_ms(500);

  //5
  fill(0x00);
  //Pyramid
    box_wireframe(0,0,0,7,4,1);
    box_wireframe(1,1,2,6,4,3);  
    box_wireframe(2,2,4,5,4,5);
    box_wireframe(3,3,6,4,4,7);

  box_walls(0,5,0,7,5,7); 
  delay_ms(500);
  //6
  
  fill(0x00);
  //Pyramid
    box_wireframe(0,0,0,7,5,1);
    box_wireframe(1,1,2,6,5,3);  
    box_wireframe(2,2,4,5,5,5);
    box_wireframe(3,3,6,4,4,7);
    
      box_walls(0,6,0,7,6,7); 
  delay_ms(500);
  //7
  fill(0x00);  
  //Pyramid
    box_wireframe(0,0,0,7,6,1);
    box_wireframe(1,1,2,6,6,3);  
   box_wireframe(2,2,4,5,5,5);
    box_wireframe(3,3,6,4,4,7);
    
  box_walls(0,7,0,7,7,7);     
  delay_ms(500);  
    
  fill(0x00);
  box_wireframe(0,0, 0, 7, 7, 1);      
  box_wireframe(1,1, 2, 6, 6, 3);      
  box_wireframe(2,2, 4, 5, 5, 5);         
  box_wireframe(3,3, 6, 4, 4, 7); 

  delay_ms(10000);   
}


void effect_intro(){
int cnt,cnt_2,time; 
 
  //Bottom To Top  
   
  for(cnt=0;cnt<=7;++cnt){
   box_wireframe(0, 0, 0, 7, 7, cnt);   
   delay_ms(2000);
  }
  for(cnt=0;cnt<7;++cnt){
  clrplane_z(cnt);
  delay_ms(2000);  
  }
  
  //Shift Things Right
    //1
  shift(AXIS_Y,-1);
  for(cnt=0;cnt<=7;++cnt){
  setvoxel(cnt,0,6);  
  }
  delay_ms(2000);  
    //2
  shift(AXIS_Y,-1);
  for(cnt=0;cnt<=7;++cnt){
  setvoxel(cnt,0,5);  
  }
  setvoxel(0,0,6);
  setvoxel(7,0,6);
  delay_ms(2000);  
    //3
  shift(AXIS_Y,-1);
  for(cnt=0;cnt<=7;++cnt){
  setvoxel(cnt,0,4);  
  }
  setvoxel(0,0,5);
  setvoxel(7,0,5);
  setvoxel(0,0,6);
  setvoxel(7,0,6);
  delay_ms(2000);

    //4
  shift(AXIS_Y,-1);
  for(cnt=0;cnt<=7;++cnt){
  setvoxel(cnt,0,3);  
  }
  setvoxel(0,0,4);
  setvoxel(7,0,4);
  setvoxel(0,0,5);
  setvoxel(7,0,5);
  setvoxel(0,0,6);
  setvoxel(7,0,6);
  delay_ms(2000);
   
    //5
  shift(AXIS_Y,-1);
  for(cnt=0;cnt<=7;++cnt){
  setvoxel(cnt,0,2);  
  }
  setvoxel(0,0,3);
  setvoxel(7,0,3);
  setvoxel(0,0,4);
  setvoxel(7,0,4);
  setvoxel(0,0,5);
  setvoxel(7,0,5);
  setvoxel(0,0,6);
  setvoxel(7,0,6);
    delay_ms(2000);
  
    //6
  shift(AXIS_Y,-1);
  for(cnt=0;cnt<=7;++cnt){
  setvoxel(cnt,0,1);  
  }
  setvoxel(0,0,2);
  setvoxel(7,0,2);
  setvoxel(0,0,3);
  setvoxel(7,0,3);
  setvoxel(0,0,4);
  setvoxel(7,0,4);
  setvoxel(0,0,5);
  setvoxel(7,0,5);
  delay_ms(2000);
  
  
    //7
  shift(AXIS_Y,-1);
  for(cnt=0;cnt<=7;++cnt){
  setvoxel(cnt,0,0);  
  }
  setvoxel(0,0,1);
  setvoxel(7,0,1);
  setvoxel(0,0,2);
  setvoxel(7,0,2);
  setvoxel(0,0,3);
  setvoxel(7,0,3);
  setvoxel(0,0,4);
  setvoxel(7,0,4);  
  setvoxel(0,0,5);
  setvoxel(7,0,5);
  delay_ms(2000);

    //Right To Left   
  for(cnt=0;cnt<=7;++cnt){
   box_wireframe(0, 0, 0, 7, cnt, 7);   
   delay_ms(2000);
  }
  for(cnt=0;cnt<7;++cnt){
  clrplane_y(cnt);
  delay_ms(2000);  
  }

  //Shift to the bottom
  for(cnt_2=6;cnt_2>=0;--cnt_2){
    shift(AXIS_Z,-1);
    for(cnt=0;cnt<=7;++cnt){
      setvoxel(cnt,cnt_2,0);  
    }
      for(cnt=6;cnt>cnt_2;--cnt){
        setvoxel(0,cnt,0);
        setvoxel(7,cnt,0);      
      }
    
      delay_ms(2000);  
  }   

  //Make All Wall Box

  for(cnt=0;cnt<=6;++cnt){
    fill(0x00);    
    box_walls(0,0,0,7,7,cnt); 
    delay_ms(2000);  
  }  
  
  time = 2000;
  for(cnt_2=0;cnt_2<5;++cnt_2){
  time = time - 300;
  //Make Box Smaller
    for(cnt=0;cnt<=3;++cnt){
      fill(0x00);
      box_walls(cnt,cnt,cnt,7-cnt,7-cnt,7-cnt);   
      delay_ms(time);
    }  
    
    //Make Box Bigger
    for(cnt=0;cnt<=3;++cnt){
      fill(0x00);
      box_walls(3-cnt,3-cnt,3-cnt,4+cnt,4+cnt,4+cnt);   
      delay_ms(time);
    }  
  }
  for(cnt_2=0;cnt_2<5;++cnt_2){
  time = time + 300;
  //Make Box Smaller
  for(cnt=0;cnt<=3;++cnt){
    fill(0x00);
    box_walls(cnt,cnt,cnt,7-cnt,7-cnt,7-cnt);   
    delay_ms(time);
  }  
  
  //Make Box Bigger
  for(cnt=0;cnt<=3;++cnt){
    fill(0x00);
    box_walls(3-cnt,3-cnt,3-cnt,4+cnt,4+cnt,4+cnt);   
    delay_ms(time);
  }  
  }  
    delay_ms(2000);
  
}

void init_LUT(unsigned char LUT[65])
{
	unsigned char i;
	float sin_of,sine;
	for (i=0;i<65;++i)
	{
		sin_of=i*PI/64; // Just need half a sin wave
		sine=sin(sin_of);
		// Use 181.0 as this squared is <32767, so we can multiply two sin or cos without overflowing an int.
		LUT[i]=sine*181.0;
	}
}

// ******************************************
// 3D addins ********************************
// ******************************************
void linespin (int iterations, int delay)
{
	float top_x, top_y, top_z, bot_x, bot_y, bot_z, sin_base;
	float center_x, center_y;

	center_x = 4;
	center_y = 4;

	int i, z;
	for (i=0;i<iterations;++i)
	{

		//printf("Sin base %f \n",sin_base);

		for (z = 0; z < 8; ++z)
		{

		sin_base = (float)i/50 + (float)z/(10+(7*sin((float)i/200)));

		top_x = center_x + sin(sin_base)*5;
		top_y = center_x + cos(sin_base)*5;
		//top_z = center_x + cos(sin_base/100)*2.5;

		bot_x = center_x + sin(sin_base+3.14)*10;
		bot_y = center_x + cos(sin_base+3.14)*10;
		//bot_z = 7-top_z;
		
		bot_z = z;
		top_z = z;

		// setvoxel((int) top_x, (int) top_y, 7);
		// setvoxel((int) bot_x, (int) bot_y, 0);

		//printf("P1: %i %i %i P2: %i %i %i \n", (int) top_x, (int) top_y, 7, (int) bot_x, (int) bot_y, 0);

		//line_3d((int) top_x, (int) top_y, (int) top_z, (int) bot_x, (int) bot_y, (int) bot_z);
		line_3d((int) top_z, (int) top_x, (int) top_y, (int) bot_z, (int) bot_x, (int) bot_y);
		}

		// delay_ms(delay);
		fill(0x00);
	}

}


void line_3d (int x1, int y1, int z1, int x2, int y2, int z2)
{
	int i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc,
	err_1, err_2, dx2, dy2, dz2;
	int pixel[3];
	pixel[0] = x1;
	pixel[1] = y1;
	pixel[2] = z1;
	dx = x2 - x1;
	dy = y2 - y1;
	dz = z2 - z1;
	x_inc = (dx < 0) ? -1 : 1;
	l = abs(dx);
	y_inc = (dy < 0) ? -1 : 1;
	m = abs(dy);
	z_inc = (dz < 0) ? -1 : 1;
	n = abs(dz);
	dx2 = l << 1;
	dy2 = m << 1;
	dz2 = n << 1;
	if ((l >= m) && (l >= n)) {
	err_1 = dy2 - l;
	err_2 = dz2 - l;
	for (i = 0; i < l; ++i) {
	//PUT_PIXEL(pixel);
	setvoxel(pixel[0],pixel[1],pixel[2]);
	//printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
	if (err_1 > 0) {
	pixel[1] += y_inc;
	err_1 -= dx2;
	}
	if (err_2 > 0) {
	pixel[2] += z_inc;
	err_2 -= dx2;
	}
	err_1 += dy2;
	err_2 += dz2;
	pixel[0] += x_inc;
	}
	} else if ((m >= l) && (m >= n)) {
	err_1 = dx2 - m;
	err_2 = dz2 - m;
	for (i = 0; i < m; ++i) {
	//PUT_PIXEL(pixel);
	setvoxel(pixel[0],pixel[1],pixel[2]);
	//printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
	if (err_1 > 0) {
	pixel[0] += x_inc;
	err_1 -= dy2;
	}
	if (err_2 > 0) {
	pixel[2] += z_inc;
	err_2 -= dy2;
	}
	err_1 += dx2;
	err_2 += dz2;
	pixel[1] += y_inc;
	}
	} else {
	err_1 = dy2 - n;
	err_2 = dx2 - n;
	for (i = 0; i < n; ++i) {
	setvoxel(pixel[0],pixel[1],pixel[2]);
	//printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
	//PUT_PIXEL(pixel);
	if (err_1 > 0) {
	pixel[1] += y_inc;
	err_1 -= dz2;
	}
	if (err_2 > 0) {
	pixel[0] += x_inc;
	err_2 -= dz2;
	}
	err_1 += dy2;
	err_2 += dx2;
	pixel[2] += z_inc;
	}
	}
	setvoxel(pixel[0],pixel[1],pixel[2]);
	//printf("Setting %i %i %i \n", pixel[0],pixel[1],pixel[2]);
	//PUT_PIXEL(pixel);
}


// ******************************************

void effect_plane_flip(unsigned char LUT[],unsigned char start,unsigned char end,int delay)
{
	unsigned char p1,p2;// point across and down on flip
	unsigned char x,y,z;
	unsigned char i;// rotational position
	unsigned char i1;// linear position
	unsigned char dir=0;//0-2  0=sidetrap 1=door 2=trap
	unsigned char rev=0;//0 for forward 1 for reverse
	unsigned char inv=0;//bit 0=Y bit 1=Z
	// Sort out dir, rev and inv for each start/end combo.
	//  There are 24, but with some neat combinations of
	// tests we can simplify so that only a max of 3 tests
	// are required for each type!
	if (start<2)// 0 or 1.  buh
	{
		if (end<4)
		{
			dir=0;
			if (end==3) inv=0x01;
		}
		else
		{
			dir=2;
			if (end==5) inv=0x01;
		}
		if (start==1) inv|=0x02;
	} else if (start<4)//2 or 3. Buh
	{
		if (end<2)// going to 0 or 1
		{
			rev=1;
			dir=0;
			if (start==3) inv=0x01;
			if (end==1) inv|=0x02;
		}
		else // going to 4 or 5
		{
			dir=1;// door moves
			if(start+end==7)//3 to 4 or 2 to 5
			{
				if (start==3)
					inv=0x02;
				else
					inv=0x01;
			}
			else//2 to 4 or 3 to 5
			{
				if (start==3)
					inv=0x03;
			}
		}
	}
	else //start is 4 or 5
	{
		if (end<2)// 0 or 1
		{
			dir=2;
			rev=1;// reverse trapdoor.  Yeah!
			if (start+end==5)//4 to 1 or 5 to 0
			{
				if (start==4)//4 to 1
					inv=0x02;
				else
					inv=0x01;
			}
			else // 4 to 0 or 5 to 1
				if (start==5)
					inv=0x03;
		}
		else//end is 2 or 3
		{
			rev=1;// all reverse
			dir=1;// all door
			if (start+end==7)//4 to 3 or 5 to 2
			{
				if (start==4) // 4 to 3
					inv=0x02;
				else
					inv=0x01;
			}
			else
			{
				if (start==5)//5 to 3
					inv=0x03;
			}
		}
	}
	// Do the actual plane drawing
	for(i=0;i<7;++i)
	{
		if (rev)// Reverse movement goes cos-sin
		{
			p2=totty_sin(LUT,i*4);// angle 0-45 degrees
			p1=totty_cos(LUT,i*4);
		}
		else
		{
			p1=totty_sin(LUT,i*4);// angle 0-45 degrees
			p2=totty_cos(LUT,i*4);
		}
		fill(0x00);
		for (i1=0;i1<8;++i1)
		{
			z=p1*i1/168;
			if (inv&0x02) z=7-z;// invert in Z axis
			y=p2*i1/168;
			if (inv&0x01) y=7-y;// invert in Y axis
			for(x=0;x<8;++x)
			{
				if(!dir)//dir=0
				{
					setvoxel(x,y,z);
				}
				else if(dir==1)
				{
					setvoxel(y,z,x);
				}
				else//dir=2
				{
					setvoxel(y,x,z);
				}
			}
		}
		delay_ms(delay);
	}
}

int totty_sin(unsigned char LUT[65],int sin_of)
{
	unsigned char inv=0;
	if (sin_of<0)
	{
		sin_of=-sin_of;
		inv=1;
	}
	sin_of&=0x7f; //127
	if (sin_of>64)
	{
		sin_of-=64;
		inv=1-inv;
	}
	if (inv)
		return -LUT[sin_of];
	else
		return LUT[sin_of];
}


int totty_cos(unsigned char LUT[65],int cos_of)
{
	unsigned char inv=0;
	cos_of+=32;// Simply rotate by 90 degrees for COS
	cos_of&=0x7f;//127
	if (cos_of>64)
	{
		cos_of-=64;
		inv=1;
	}
	if (inv)
		return -LUT[cos_of];
	else
		return LUT[cos_of];
}

// **********************************************************

//volatile const unsigned char bitmaps[16][8] = {
const unsigned char bitmaps[16][8] PROGMEM = {
	{0xc3,0xc3,0x00,0x18,0x18,0x81,0xff,0x7e}, // smiley 3 small
	{0x3c,0x42,0x81,0x81,0xc3,0x24,0xa5,0xe7}, // Omega
	{0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81}, // X
	{0xBD,0xA1,0xA1,0xB9,0xA1,0xA1,0xA1,0x00}, // ifi
	{0xEF,0x48,0x4B,0x49,0x4F,0x00,0x00,0x00}, // TG
	{0x38,0x7f,0xE6,0xC0,0xE6,0x7f,0x38,0x00}, // Commodore symbol
	{0x00,0x22,0x77,0x7f,0x3e,0x3e,0x1c,0x08}, // Heart
	{0x1C,0x22,0x55,0x49,0x5d,0x22,0x1c,0x00}, // face
	{0x37,0x42,0x22,0x12,0x62,0x00,0x7f,0x00}, // ST
	{0x89,0x4A,0x2c,0xF8,0x1F,0x34,0x52,0x91}, // STAR
	{0x18,0x3c,0x7e,0xdb,0xff,0x24,0x5a,0xa5}, // Space Invader
	{0x00,0x9c,0xa2,0xc5,0xc1,0xa2,0x9c,0x00}, // Fish
        {0x00,0x08,0x1C,0x3E,0x7F,0x1C,0x1C,0x1C}, // Up-Arrow
        {0x00,0x1C,0x1C,0x1C,0x7F,0x3E,0x1C,0x08}, // Down-Arrow
        {0x00,0x08,0x18,0x3F,0x7F,0x3F,0x18,0x08}, // Left-Arrow
        {0x00,0x08,0x0C,0x7E,0x7F,0x7E,0x0C,0x08}  // Right-Arrow
        
        
};
unsigned char font_getbitmappixel (char bitmap, char x, char y)
  {
//      uint_fast8_t tmp = bitmaps[(uint_fast8_t)bitmap][(uint_fast8_t)x];
      uint_fast8_t tmp = pgm_read_byte(&bitmaps[(uint_fast8_t)bitmap][(uint_fast8_t)x]);
    return (tmp >> y) & 0x01;
  }

const unsigned char paths[44] PROGMEM = {0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x67,0x57,0x47,0x37,0x27,0x17,
0x04,0x03,0x12,0x21,0x30,0x40,0x51,0x62,0x73,0x74,0x65,0x56,0x47,0x37,0x26,0x15}; // circle, len 16, offset 28

void font_getpath (unsigned char path, unsigned char *destination, int length)
{
	int i;
	int offset = 0;
	
	if (path == 1)
		offset=28;
	
	for (i = 0; i < length; ++i)
		destination[i] = pgm_read_byte(&paths[i+offset]);
}

// ************************************************************

void effect_pathmove (unsigned char *path, int length)
{
	int i,z;
	unsigned char state;
	
	for (i=(length-1);i>=1;--i)
	{
		for (z=0;z<8;++z)
		{
		
			state = getvoxel(((path[(i-1)]>>4) & 0x0f), (path[(i-1)] & 0x0f), z);
			altervoxel(((path[i]>>4) & 0x0f), (path[i] & 0x0f), z, state);
		}
	}
	for (i=0;i<8;++i)
		clrvoxel(((path[0]>>4) & 0x0f), (path[0] & 0x0f),i);
}

void effect_rand_patharound (int iterations, int delay)
{
	int z, dz, i;
	z = 4;
	unsigned char path[28];
	
	font_getpath(0,path,28);
	
	for (i = 0; i < iterations; ++i)
	{
		dz = ((rand()%3)-1);
		z += dz;
		
		if (z>7)
			z = 7;
			
		if (z<0)
			z = 0;
		
		effect_pathmove(path, 28);
		setvoxel(0,7,z);
		delay_ms(delay);
	}
}

// Flip the cube 180 degrees along the x axis
void mirror_x (void)
{
    unsigned char buffer[8][8];
    unsigned char y,z;

    memcpy(buffer, (char*)cube, 64); // copy the current cube into a buffer.

    fill(0x00);

    for (z=0; z<8; ++z)
    {
        for (y=0; y<8; ++y)
        {
            cube[z][y] = flipbyte(buffer[z][y]);
        }
    }
}
// Flip the cube 180 degrees along the y axis.
void mirror_y (void)
{
    unsigned char buffer[8][8];
    unsigned char x,y,z;

    memcpy(buffer, (char*)cube, 64); // copy the current cube into a buffer.

    fill(0x00);
    for (z=0; z<8; ++z)
    {
        for (y=0; y<8; ++y)
        {
            for (x=0; x<8; ++x)
            {
                if (buffer[z][y] & (0x01 << x))
                    setvoxel(x,7-y,z);
            }
        }
    }

}
// flip the cube 180 degrees along the z axis
void mirror_z (void)
{
    unsigned char buffer[8][8];
    unsigned char z, y;

    memcpy(buffer, (char*)cube, 64); // copy the current cube into a buffer.

    for (y=0; y<8; ++y)
    {
        for (z=0; z<8; ++z)
        {
            cube[7-z][y] = buffer[z][y];
        }
    }
}
