#include<iostream>
#include<vector>

#define HEIGHT 5
#define WIDTH 4

using namespace std;

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
    for(int i=0; i<HEIGHT; i++)
        for(int j=0; j<WIDTH; j++)
            config[i][j] = con[i][j];

    distance= dis;
}

bool Board::isValid()
{
    if(checkLargeSqr()==0)
        return 0;

    for(int i=1; i<6; i++)
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
        for(size_t j=0; j<WIDTH; j++)
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
    for(size_t x=6; x<10; x++)
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
        size_t i=0, j;
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
    for(size_t i=0; i<HEIGHT; i++)
    {
        for(size_t j=0; j<WIDTH; j++)
        {
            cout<< config[i][j]<< '\t';
        }
        cout<< "\n";
    }
    cout<< "\n";
}

int main()
{
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
    for(size_t i=0; i<nextConf.size(); i++)
        nextConf[i].printConfig();
}
