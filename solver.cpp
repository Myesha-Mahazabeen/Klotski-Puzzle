// Myesha Mahazabeen, EMPLID: 24005884

/* compute optimal solutions for sliding block puzzle. */
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <cstdlib>   /* for atexit() */
#include <algorithm>
using std::swap;
#include <cassert>
#include<iostream>
#include<vector>

using namespace std;
/* SDL reference: https://wiki.libsdl.org/CategoryAPI */

/* initial size; will be set to screen size after window creation. */
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
int fcount = 0;
int mousestate = 0;
SDL_Point lastm = {0,0}; /* last mouse coords */
SDL_Rect bframe; /* bounding rectangle of board */
static const int ep = 2; /* epsilon offset from grid lines */
#define HEIGHT 5
#define WIDTH 4

bool init(); /* setup SDL */
void initBlocks();

// #define FULLSCREEN_FLAG SDL_WINDOW_FULLSCREEN_DESKTOP
#define FULLSCREEN_FLAG 0

class Board
{
private:
    int config[HEIGHT][WIDTH];
    int distance;
public:
    Board(int con[HEIGHT][WIDTH], int dis=0);
    bool isValid();
    bool checkLargeSqr();           // check if the large square is in valid place
    bool checkRectangle(int x);     // check if the rectangles are in valid place
    bool checkEmpty();              // check if the empty cells in valid place
    bool checkSmallSqrs();          // check if small squares are in valid place
    bool isFinal();                 // check if the configuration is in final state
	vector<Board> makeMove();		// makes all the possible moves
	void copyConf(int tmp[5][4]);	// copies one board's configuration
    vector<Board> moveLargeSqr();	// move large square and return the valid configs
    vector<Board> moveSmallSqr();	// move small squares and return the valid configs
    vector<Board> moveRectangle();	// move rectangles and return the valid configs
	void printConfig();
};

Board::Board(int con[HEIGHT][WIDTH], int dis)
{
    for(size_t i=0; i<HEIGHT; i++)
        for(size_t j=0; j<WIDTH; j++)
            config[i][j] = con[i][j];

    distance= dis;
}

bool Board::isValid()
{
    if(checkLargeSqr()==0)
        return 0;

    for(size_t i=1; i<6; i++)
        if(checkRectangle(i)==0)
            return 0;
    
    if(checkSmallSqrs()==0)
        return 0;

    if(checkEmpty()==0)
        return 0;

    return 1;
}

bool Board::checkLargeSqr()
{
    for(size_t i=0; i<HEIGHT; i++)
    {
        for(size_t j=0; j<WIDTH; j++)
            if(config[i][j]==0)
            {
                // checking if all the 0's align
                if((i+1<HEIGHT && config[i+1][j]==0) && (j+1<WIDTH && config[i][j+1]==0) && config[i+1][j+1]==0)
                    return 1;

                else
                    return 0;
            }
    }
    return 0;
}

bool Board::checkRectangle(int x)
{
    for(size_t i=0; i<HEIGHT; i++)
    {
        for(size_t j=0; j<WIDTH; j++)
            if(config[i][j]==x)
            {
                // checking if all the x's align
                if((i+1<HEIGHT && config[i+1][j]==x) || (j+1<WIDTH && config[i][j+1]==x))
                    return 1;

                else
                    return 0;
            }
    }
    return 0;
}

bool Board::checkSmallSqrs()
{
    bool smlSqr[4] = {0, 0, 0, 0};
    for(size_t i=0; i<HEIGHT; i++)
        for(size_t j=0; j<WIDTH; j++)
            if(config[i][j]==6)
                smlSqr[0]=1;
            else if(config[i][j]==7)
                smlSqr[1]=1;
            else if(config[i][j]==8)
                smlSqr[2]=1;
            else if(config[i][j]==9)
                smlSqr[3]=1;

    for(size_t i=0; i<4; i++)
        if(smlSqr[i]==0)
            return 0;

    return 1;
}

bool Board::checkEmpty()
{
    int empCnt = 0;
    for(size_t i=0; i<HEIGHT; i++)
        for(size_t j=0; j<WIDTH; j++)
            if(config[i][j]==-1)
                empCnt++;

    return (empCnt==2);
}

bool Board::isFinal()
{
    if(config[3][1]==0 && config[3][2]==0 && config[4][1]==0 && config[4][2]==0)
        return 1;
    return 0;
}

vector<Board> Board::makeMove()
{
    vector<Board> con;
    vector<Board> tmp;
    tmp = moveLargeSqr();
    for(size_t i=0; i<tmp.size(); i++)
        con.push_back(tmp[i]);
    tmp= moveSmallSqr();
    for(size_t i=0; i<tmp.size(); i++)
        con.push_back(tmp[i]);
    tmp= moveRectangle();
    for(size_t i=0; i<tmp.size(); i++)
        con.push_back(tmp[i]);
    return con;
}
void Board::copyConf(int tmp[5][4])
{
    for(size_t i=0; i<HEIGHT; i++)
        for(int j=0; j<WIDTH; j++)
            tmp[i][j] = config[i][j];
}
vector<Board> Board::moveLargeSqr()
{
    vector<Board> con;
    size_t i, j;
    bool flag=0;
    //finding position of large square
    for(i=0; i<HEIGHT; i++)
    {
        for(j=0; j<WIDTH; j++)
            if(config[i][j]==0)
            {
                flag=1;
                break;
            }
        if(flag==1)
            break;
    }

    int tmp[HEIGHT][WIDTH];
    
    //move up
    if(i-1>=0)
    {
        copyConf(tmp);
        tmp[i-1][j]= 0;
        tmp[i-1][j+1]= 0;
        tmp[i+1][j]= -1;
        tmp[i+1][j+1]= -1;
        Board upMove = Board(tmp, distance+1);
        if(upMove.isValid()) con.push_back(upMove);
    }
    //moving down
    if(i+2<HEIGHT)
    {
        copyConf(tmp);
        tmp[i+2][j]= 0;
        tmp[i+2][j+1]= 0;
        tmp[i][j]= -1;
        tmp[i][j+1]= -1;
        Board downMove = Board(tmp, distance+1);
        if(downMove.isValid()) con.push_back(downMove);
    }
    //move left
    if(j-1>=0)
    {
        copyConf(tmp);
        tmp[i][j-1]= 0;
        tmp[i+1][j-1]= 0;
        tmp[i][j+1]= -1;
        tmp[i+1][j+1]= -1;
        Board leftMove = Board(tmp, distance+1);
        if(leftMove.isValid()) con.push_back(leftMove);
    }
    //move right
    if(j+2<WIDTH)
    {
        copyConf(tmp);
        tmp[i][j+2]= 0;
        tmp[i+1][j+2]= 0;
        tmp[i][j]= -1;
        tmp[i+1][j]= -1;
        Board rightMove = Board(tmp, distance+1);
        if(rightMove.isValid()) con.push_back(rightMove);
    }
    return con;
}

vector<Board> Board::moveSmallSqr()
{
    vector<Board>con;
    int tmp[HEIGHT][WIDTH];
    for(int x=6; x<10; x++)
    {
        size_t i, j;
        bool flag= 0;
        for(i=0; i<HEIGHT; i++)
        {
            for(j=0; j<WIDTH; j++)
            {
                if(config[i][j]==x)
                {
                    flag= 1;
                    break;
                }
            }
            if(flag==1)
                break;
        }
        //move up
        if(i-1>=0 && config[i-1][j]==-1)
        {
            copyConf(tmp);
            tmp[i-1][j]= x;
            tmp[i][j]= -1;
            Board upOneMove = Board(tmp, distance+1);
            if(upOneMove.isValid()) con.push_back(upOneMove);

            if(i-2>=0 && config[i-2][j]==-1)
            {
                copyConf(tmp);
                tmp[i-2][j]= x;
                tmp[i][j]= -1;
                Board upTwoMove = Board(tmp, distance+1);
                if(upTwoMove.isValid()) con.push_back(upTwoMove);
            }
        }
        //move down
        if(i+1<HEIGHT && config[i+1][j]==-1)
        {
            copyConf(tmp);
            tmp[i+1][j]= x;
            tmp[i][j]= -1;
            Board downOneMove = Board(tmp, distance+1);
            if(downOneMove.isValid()) con.push_back(downOneMove);

            if(i+2<HEIGHT && config[i+2][j]==-1)
            {
                copyConf(tmp);
                tmp[i+2][j]= x;
                tmp[i][j]= -1;
                Board downTwoMove = Board(tmp, distance+1);
                if(downTwoMove.isValid()) con.push_back(downTwoMove);
            }
        }
        //move left
        if(j-1>=0 && config[i][j-1]==-1)
        {
            copyConf(tmp);
            tmp[i][j-1]= x;
            tmp[i][j]= -1;
            Board leftOneMove = Board(tmp, distance+1);
            if(leftOneMove.isValid()) con.push_back(leftOneMove);

            if(j-2>=0 && config[i][j-2]==-1)
            {
                copyConf(tmp);
                tmp[i][j-2]= x;
                tmp[i][j]= -1;
                Board leftTwoMove = Board(tmp, distance+1);
                if(leftTwoMove.isValid()) con.push_back(leftTwoMove);
            }
        }
        //move right
        if(j+1<HEIGHT && config[i][j+1]==-1)
        {
            copyConf(tmp);
            tmp[i][j+1]= x;
            tmp[i][j]= -1;
            Board rightOneMove = Board(tmp, distance+1);
            if(rightOneMove.isValid()) con.push_back(rightOneMove);

            if(j+2<HEIGHT && config[i][j+2]==-1)
            {
                copyConf(tmp);
                tmp[i][j+2]= x;
                tmp[i][j]= -1;
                Board rightTwoMove = Board(tmp, distance+1);
                if(rightTwoMove.isValid()) con.push_back(rightTwoMove);
            }
        }
    }
    return con;
}

vector<Board> Board::moveRectangle()
{
    vector<Board> con;
    int tmp[HEIGHT][WIDTH];
    for(int x=1; x<6; x++)
    {
        bool flag= 0;
        bool vertical = 1;
        int i=0, j;
        for(i=0; i<HEIGHT; i++)
        {
            for(j=0; j<WIDTH; j++)
            {
                if(config[i][j]==x)
                {
                    if(j+1<WIDTH && config[i][j+1]==x)
                        vertical= 0;
                    flag= 1;
                    break;
                }
            }
            if(flag==1)
                break;
        }
        if(vertical)
        {
            //move up
            if(i-1>=0 && config[i-1][j]==-1)
            {
                copyConf(tmp);
                tmp[i-1][j]= x;
                tmp[i+1][j]= -1;
                Board upOneMove = Board(tmp, distance+1);
                if(upOneMove.isValid()) con.push_back(upOneMove);

                if(i-2>=0 && config[i-2][j]==-1)
                {
                    copyConf(tmp);
                    tmp[i-2][j]= x;
                    tmp[i-1][j]= x;
                    tmp[i][j]= -1;
                    tmp[i+1][j]= -1;
                    Board upTwoMove = Board(tmp, distance+1);
                    if(upTwoMove.isValid()) con.push_back(upTwoMove);
                }
            }
            //move down
            if(i+2<HEIGHT && config[i+2][j]==-1)
            {
                copyConf(tmp);
                tmp[i+2][j]= x;
                tmp[i][j]= -1;
                Board downOneMove = Board(tmp, distance+1);
                if(downOneMove.isValid()) con.push_back(downOneMove);

                if(i+3<HEIGHT && config[i+3][j]==-1)
                {
                    copyConf(tmp);
                    tmp[i+3][j]= x;
                    tmp[i+2][j]= x;
                    tmp[i][j]= -1;
                    tmp[i+1][j]= -1;
                    Board downTwoMove = Board(tmp, distance+1);
                    if(downTwoMove.isValid()) con.push_back(downTwoMove);
                }
            }
            //move left
            if(j-1>=0 && config[i][j-1]==-1 && config[i+1][j-1]==-1)
            {
                copyConf(tmp);
                tmp[i][j-1]= x;
                tmp[i+1][j-1]= x;
                tmp[i][j]= -1;
                tmp[i+1][j]= -1;
                Board leftOneMove = Board(tmp, distance+1);
                if(leftOneMove.isValid()) con.push_back(leftOneMove);
            }
            //move right
            if(j+1<WIDTH && config[i][j+1]==-1 && config[i+1][j+1]==-1)
            {
                copyConf(tmp);
                tmp[i][j+1]= x;
                tmp[i+1][j+1]= x;
                tmp[i][j]= -1;
                tmp[i+1][j]= -1;
                Board rightOneMove = Board(tmp, distance+1);
                if(rightOneMove.isValid()) con.push_back(rightOneMove);
            }
        }
        else    // horizontal
        {
            //move up
            if(i-1>=0 && config[i-1][j]==-1 && config[i-1][j+1]==-1)
            {
                copyConf(tmp);
                tmp[i-1][j]= x;
                tmp[i-1][j+1]= x;
                tmp[i][j]= -1;
                tmp[i][j+1]=-1;
                Board upOneMove = Board(tmp, distance+1);
                if(upOneMove.isValid()) con.push_back(upOneMove);
            }
            //move down
            if(i+1<HEIGHT && config[i+1][j]==-1 && config[i+1][j+1]==-1)
            {
                copyConf(tmp);
                tmp[i+1][j]= x;
                tmp[i+1][j+1]= x;
                tmp[i][j]= -1;
                tmp[i][j+1]= -1;
                Board downOneMove = Board(tmp, distance+1);
                if(downOneMove.isValid()) con.push_back(downOneMove);
            }
            //move left
            if(j-1>=0 && config[i][j-1]==-1)
            {
                copyConf(tmp);
                tmp[i][j-1]= x;
                tmp[i][j+1]= -1;
                Board leftOneMove = Board(tmp, distance+1);
                if(leftOneMove.isValid()) con.push_back(leftOneMove);

                if(j-2>=0 && config[i][j-2]==-1)
                {
                    copyConf(tmp);
                    tmp[i][j-2]= x;
                    tmp[i][j-1]= x;
                    tmp[i][j]= -1;
                    tmp[i][j+1]= -1;
                    Board leftTwoMove = Board(tmp, distance+1);
                    if(leftTwoMove.isValid()) con.push_back(leftTwoMove);
                }
            }
            //move right
            if(j+2>=0 && config[i][j+2]==-1)
            {
                copyConf(tmp);
                tmp[i][j+2]= x;
                tmp[i][j]= -1;
                Board rightOneMove = Board(tmp, distance+1);
                if(rightOneMove.isValid()) con.push_back(rightOneMove);

                if(j+3>=0 && config[i][j+3]==-1)
                {
                    copyConf(tmp);
                    tmp[i][j+3]= x;
                    tmp[i][j+2]= x;
                    tmp[i][j]= -1;
                    tmp[i][j+1]= -1;
                    Board rightTwoMove = Board(tmp, distance+1);
                    if(rightTwoMove.isValid()) con.push_back(rightTwoMove);
                }
            }
        }
    }
    // cout<< "size " <<con.size()<< endl;
    return con;
}

void Board::printConfig()
{
    cout<< "Distance: "<<distance<< "\n";
    for(int i=0; i<HEIGHT; i++)
    {
        for(int j=0; j<WIDTH; j++)
        {
            cout<< config[i][j]<< '\t';
        }
        cout<< "\n";
    }
    cout<< "\n";
}
/* NOTE: ssq == "small square", lsq == "large square" */
enum bType {hor,ver,ssq,lsq};
struct block {
	SDL_Rect R; /* screen coords + dimensions */
	bType type; /* shape + orientation */
	/* TODO: you might want to add other useful information to
	 * this struct, like where it is attached on the board.
	 * (Alternatively, you could just compute this from R.x and R.y,
	 * but it might be convenient to store it directly.) */
	void rotate() /* rotate rectangular pieces */
	{
		if (type != hor && type != ver) return;
		type = (type==hor)?ver:hor;
		swap(R.w,R.h);
	}
};

#define NBLOCKS 10
block B[NBLOCKS];
block* dragged = NULL;

block* findBlock(int x, int y);
void close(); /* call this at end of main loop to free SDL resources */
SDL_Window* gWindow = 0; /* main window */
SDL_Renderer* gRenderer = 0;

bool init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL_Init failed.  Error: %s\n", SDL_GetError());
		return false;
	}
	/* NOTE: take this out if you have issues, say in a virtualized
	 * environment: */
	if(!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) {
		printf("Warning: vsync hint didn't work.\n");
	}
	/* create main window */
	gWindow = SDL_CreateWindow("Sliding block puzzle solver",
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								SCREEN_WIDTH, SCREEN_HEIGHT,
								SDL_WINDOW_SHOWN|FULLSCREEN_FLAG);
	if(!gWindow) {
		printf("Failed to create main window. SDL Error: %s\n", SDL_GetError());
		return false;
	}
	/* set width and height */
	SDL_GetWindowSize(gWindow, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	/* setup renderer with frame-sync'd drawing: */
	gRenderer = SDL_CreateRenderer(gWindow, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!gRenderer) {
		printf("Failed to create renderer. SDL Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawBlendMode(gRenderer,SDL_BLENDMODE_BLEND);

	initBlocks();
	return true;
}

/* TODO: you'll probably want a function that takes a state / configuration
 * and arranges the blocks in accord.  This will be useful for stepping
 * through a solution.  Be careful to ensure your underlying representation
 * stays in sync with what's drawn on the screen... */

void initBlocks()
{
	int& W = SCREEN_WIDTH;
	int& H = SCREEN_HEIGHT;
	int h = H*3/4;
	int w = 4*h/5;
	int u = h/5-2*ep;
	int mw = (W-w)/2;
	int mh = (H-h)/2;

	/* setup bounding rectangle of the board: */
	bframe.x = (W-w)/2;
	bframe.y = (H-h)/2;
	bframe.w = w;
	bframe.h = h;

	/* NOTE: there is a tacit assumption that should probably be
	 * made explicit: blocks 0--4 are the rectangles, 5-8 are small
	 * squares, and 9 is the big square.  This is assumed by the
	 * drawBlocks function below. */

	for (size_t i = 0; i < 5; i++) {
		B[i].R.x = (mw-2*u)/2;
		B[i].R.y = mh + (i+1)*(u/5) + i*u;
		B[i].R.w = 2*(u+ep);
		B[i].R.h = u;
		B[i].type = hor;
	}
	B[4].R.x = mw+ep;
	B[4].R.y = mh+ep;
	B[4].R.w = 2*(u+ep);
	B[4].R.h = u;
	B[4].type = hor;
	/* small squares */
	for (size_t i = 0; i < 4; i++) {
		B[i+5].R.x = (W+w)/2 + (mw-2*u)/2 + (i%2)*(u+u/5);
		B[i+5].R.y = mh + ((i/2)+1)*(u/5) + (i/2)*u;
		B[i+5].R.w = u;
		B[i+5].R.h = u;
		B[i+5].type = ssq;
	}
	B[9].R.x = B[5].R.x + u/10;
	B[9].R.y = B[7].R.y + u + 2*u/5;
	B[9].R.w = 2*(u+ep);
	B[9].R.h = 2*(u+ep);
	B[9].type = lsq;
}

void drawBlocks()
{
	/* rectangles */
	SDL_SetRenderDrawColor(gRenderer, 0x43, 0x4c, 0x5e, 0xff);
	for (size_t i = 0; i < 5; i++) {
		SDL_RenderFillRect(gRenderer,&B[i].R);
	}
	/* small squares */
	SDL_SetRenderDrawColor(gRenderer, 0x5e, 0x81, 0xac, 0xff);
	for (size_t i = 5; i < 9; i++) {
		SDL_RenderFillRect(gRenderer,&B[i].R);
	}
	/* large square */
	SDL_SetRenderDrawColor(gRenderer, 0xa3, 0xbe, 0x8c, 0xff);
	SDL_RenderFillRect(gRenderer,&B[9].R);
}

/* return a block containing (x,y), or NULL if none exists. */
block* findBlock(int x, int y)
{
	/* NOTE: we go backwards to be compatible with z-order */
	for (int i = NBLOCKS-1; i >= 0; i--) {
		if (B[i].R.x <= x && x <= B[i].R.x + B[i].R.w &&
				B[i].R.y <= y && y <= B[i].R.y + B[i].R.h)
			return (B+i);
	}
	return NULL;
}

void close()
{
	SDL_DestroyRenderer(gRenderer); gRenderer = NULL;
	SDL_DestroyWindow(gWindow); gWindow = NULL;
	SDL_Quit();
}

void render()
{
	/* draw entire screen to be black: */
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(gRenderer);

	/* first, draw the frame: */
	int& W = SCREEN_WIDTH;
	int& H = SCREEN_HEIGHT;
	int w = bframe.w;
	int h = bframe.h;
	SDL_SetRenderDrawColor(gRenderer, 0x39, 0x39, 0x39, 0xff);
	SDL_RenderDrawRect(gRenderer, &bframe);
	/* make a double frame */
	SDL_Rect rframe(bframe);
	int e = 3;
	rframe.x -= e; 
	rframe.y -= e;
	rframe.w += 2*e;
	rframe.h += 2*e;
	SDL_RenderDrawRect(gRenderer, &rframe);

	/* draw some grid lines: */
	SDL_Point p1,p2;
	SDL_SetRenderDrawColor(gRenderer, 0x19, 0x19, 0x1a, 0xff);
	/* vertical */
	p1.x = (W-w)/2;
	p1.y = (H-h)/2;
	p2.x = p1.x;
	p2.y = p1.y + h;
	for (size_t i = 1; i < 4; i++) {
		p1.x += w/4;
		p2.x += w/4;
		SDL_RenderDrawLine(gRenderer,p1.x,p1.y,p2.x,p2.y);
	}
	/* horizontal */
	p1.x = (W-w)/2;
	p1.y = (H-h)/2;
	p2.x = p1.x + w;
	p2.y = p1.y;
	for (size_t i = 1; i < 5; i++) {
		p1.y += h/5;
		p2.y += h/5;
		SDL_RenderDrawLine(gRenderer,p1.x,p1.y,p2.x,p2.y);
	}
	SDL_SetRenderDrawColor(gRenderer, 0xd8, 0xde, 0xe9, 0x7f);
	SDL_Rect goal = {bframe.x + w/4 + ep, bframe.y + 3*h/5 + ep,
	                 w/2 - 2*ep, 2*h/5 - 2*ep};
	SDL_RenderDrawRect(gRenderer,&goal);

	/* now iterate through and draw the blocks */
	drawBlocks();
	/* finally render contents on screen, which should happen once every
	 * vsync for the display */
	SDL_RenderPresent(gRenderer);
}

void snap(block* b)
{
	/* TODO: once you have established a representation for configurations,
	 * you should update this function to make sure the configuration is
	 * updated when blocks are placed on the board, or taken off.  */
	assert(b != NULL);
	/* upper left of grid element (i,j) will be at
	 * bframe.{x,y} + (j*bframe.w/4,i*bframe.h/5) */
	/* translate the corner of the bounding box of the board to (0,0). */
	int x = b->R.x - bframe.x;
	int y = b->R.y - bframe.y;
	int uw = bframe.w/4;
	int uh = bframe.h/5;
	/* NOTE: in a perfect world, the above would be equal. */
	int i = (y+uh/2)/uh; /* row */
	int j = (x+uw/2)/uw; /* col */
	if (0 <= i && i < 5 && 0 <= j && j < 4) {
		b->R.x = bframe.x + j*uw + ep;
		b->R.y = bframe.y + i*uh + ep;
	}
}

int main(int argc, char *argv[])
{
    /* testing movements */
    int initialConfig[HEIGHT][WIDTH]= {
        {0, 0, 1, 2},
        {0, 0, 1, 2},
        {-1, -1, 3, 3},
        {6, 8, 9, 4},
        {7, 5, 5, 4}
    };
    Board initalBoard = Board(initialConfig);
    cout<< initalBoard.isValid()<< endl;
    vector<Board> nextConf = initalBoard.makeMove();
    initalBoard.printConfig();
    for(size_t i=0; i<nextConf.size(); i++){
        nextConf[i].printConfig();
    }


	/* TODO: add option to specify starting state from cmd line? */
	/* start SDL; create window and such: */
	if(!init()) {
		printf( "Failed to initialize from main().\n" );
		return 1;
	}
	atexit(close);
	bool quit = false; /* set this to exit main loop. */
	SDL_Event e;
	/* main loop: */
	while(!quit) {
		/* handle events */
		while(SDL_PollEvent(&e) != 0) {
			/* meta-q in i3, for example: */
			if(e.type == SDL_MOUSEMOTION) {
				if (mousestate == 1 && dragged) {
					int dx = e.button.x - lastm.x;
					int dy = e.button.y - lastm.y;
					lastm.x = e.button.x;
					lastm.y = e.button.y;
					dragged->R.x += dx;
					dragged->R.y += dy;
				}
			} else if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.button == SDL_BUTTON_RIGHT) {
					block* b = findBlock(e.button.x,e.button.y);
					if (b) b->rotate();
				} else {
					mousestate = 1;
					lastm.x = e.button.x;
					lastm.y = e.button.y;
					dragged = findBlock(e.button.x,e.button.y);
				}
				/* XXX happens if during a drag, someone presses yet
				 * another mouse button??  Probably we should ignore it. */
			} else if (e.type == SDL_MOUSEBUTTONUP) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					mousestate = 0;
					lastm.x = e.button.x;
					lastm.y = e.button.y;
					if (dragged) {
						/* snap to grid if nearest location is empty. */
						snap(dragged);
					}
					dragged = NULL;
				}
			} else if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_q:
						quit = true;
						break;
					case SDLK_LEFT:
						/* TODO: show previous step of solution */
						break;
					case SDLK_RIGHT:
						/* TODO: show next step of solution */
						break;
					case SDLK_p:
						/* TODO: print the state to stdout
						 * (maybe for debugging purposes...) */
						break;
					case SDLK_s:
						/* TODO: try to find a solution */
						break;
					default:
						break;
				}
			}
		}
		fcount++;
		render();
	}

	printf("total frames rendered: %i\n",fcount);
	return 0;
}
