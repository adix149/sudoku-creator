#include<iostream>
#include<vector>
#include<fstream>
#include<string.h>
#include<algorithm>     //std::random_shuffle
#include<ctime>         // std::time
#include<cstdlib>  		// std::rand, std::srand




using namespace std;


class Cell
{
private:

    int val;
    int row;
    int col;
public:
    vector<int> PossVal;          //PossVal vector holds the possible numbers for that cell
    
//PossValArray[9] holds the number of the possible values and 
//remaining array hold the Possibility of a particular number for that cell. 
//For example if the cell's possible values are 3,6,7,9 the array is populated as 0,0,1,0,0,1,1,0,1    
    int PossValArray[10];        
    Cell()                        //Default constructor with default values
    {
        val=0;
        row=-1;
        col =-1;
        for(int i=1;i<10;i++)
        {
            PossVal.push_back(i);    //For the empty cell the possible values are from 1 to 9
        }
        for(int j=0;j<10;j++)
            PossValArray[j]=1;

    }
    void showcell()                  //Prints the value in the cell to Std out
    {

        cout<<"The cell at ("<<row+1<<","<<col+1<<")"<<"has the value"<<val<<endl;

    }
    void setval(int i, int k, int d)   // Set the value at i,j with d
    {
        row=i;
        col=k;
        val=d;

        if((val>0)&&(val<10))
        {
            PossVal.clear();
            for(int j=0;j<10;j++)
                PossValArray[j]=0;   //since the val is set the remaining possible values are not applicable. Hence made Zero
            PossValArray[val]=1;	 //Only the val assigned is possible. Hence it's 1
        }

    }
    void setval(int i)
    {
        val=i;

        if((val>0)&&(val<10))
        {
            PossVal.clear();
            for(int j=0;j<10;j++)
                PossValArray[j]=0; //since the val is set the remaining possible values are not applicable. Hence made Zero
            PossValArray[val]=1;   //Only the val assigned is possible. Hence it's 1
        }

    }
    int giveval()
    {
        return val;
    }
    
/*
    * Box is the 3x3 square to which the cell belongs to.
    * That value is computed and returned to boxrow and boxcol
*/
    int boxrow()                 
    {
        int a= row/3;
        return a*3;
    }
    int boxcol()
    {
        int a= col/3;
        return a*3;
    }

};

class Table
{
public:

    Cell c[9][9];
//Get the data from the input file
    void get_table()               
    {
        int data;
        fstream infile("Null_input.txt");
        if(infile.is_open())
        {

            infile>>data;
            for(int i=0;i<9;i++)
            {
                for(int j=0;j<9;j++)
                {
                    c[i][j].setval(i,j,data);
                    if(!(infile>>data))
                        break;
                    else
                        continue;
                }

            }

            infile.close();
        }
    }
//Prints the table to the file
    void show_table()
    {
        static int cnt=0;
        ofstream outfile;
        if(cnt==0)
        {
            outfile.open("exout2.txt",ios::trunc);
        }
        else
            outfile.open("exout2.txt",ios::app);
        if(outfile.is_open())
        {
            outfile.seekp(0,ios::end);
            for(int row=0;row<9;row++)
            {
                for(int col=0;col<9;col++)
                {
                    outfile<<c[row][col].giveval()<<" ";
                    if(col==2||col==5)
                        outfile<<" | ";

                }
                if(row==2||row==5)
                    outfile<<endl<<"-----------------------"<<endl;
                outfile<<endl;

            }
            outfile<<endl<<endl<<"***************************************************"<<endl;
            outfile.flush();
            outfile.close();
            cnt++;
        }
    }
 
//Update the possible values for the each cell
    void update_possible_vals()
    {


        for(int row=0;row<9;row++)
        {
            for(int col=0;col<9;col++)
            {
                if(c[row][col].giveval()==0)        //checking only the cells without value
                {
                    for(int i=1;i<10;i++)
                    {
                        c[row][col].PossVal.push_back(i);  //Initially all values are possible
                    }
                    for(int j=0;j<10;j++)
                        c[row][col].PossValArray[j]=1;
					//Scans the row and removes already available values in that row from the PossvalArray
                    //row scan

                    for(int i=0;i<9;i++)
                    {
                        if(c[row][i].giveval()!=0&&c[row][col].PossValArray[c[row][i].giveval()] ==1)
                        {

                            c[row][col].PossValArray[c[row][i].giveval()]=0; 

                        }

                    }
					//Scans the col and removes already available values in that col from the PossvalArray
                    //col scan

                    for(int i=0;i<9;i++)
                    {
                        if(c[i][col].giveval()!=0&&c[row][col].PossValArray[c[i][col].giveval()] ==1)
                        {

                            c[row][col].PossValArray[c[i][col].giveval()]=0;

                        }
                    }
                    
					//Scans the Box and removes already available values in that Box from the PossvalArray
                    //Box scan

                    for(int i=c[row][col].boxrow();i<(c[row][col].boxrow()+3);i++)
                    {
                        for(int j=c[row][col].boxcol();j<(c[row][col].boxcol()+3);j++)
                        {
                            if(c[i][j].giveval()!=0&&c[row][col].PossValArray[c[i][j].giveval()] ==1)
                            {

                                c[row][col].PossValArray[c[i][j].giveval()]=0;

                            }
                        }
                    }


					//Syncing the PossValArray with PossVal to ensure PossVal has only the elemnts as suggested by PossValArray

                    c[row][col].PossVal.clear();
                    for(int elem=1;elem<10;elem++)
                    {
                        if(c[row][col].PossValArray[elem]==1)
                            c[row][col].PossVal.push_back(elem);
                    }



                }
            }
        }
    }
    
/*
 * Solve cell assigns cells which have only one possible value 
*/
    int solve_cell()      
    {

        ofstream outfile;
        outfile.open("exout2.txt",ios::app);
        int cnt=0;
        for(int row=0;row<9;row++)
        {
            for(int col=0;col<9;col++)
            {

                if(c[row][col].PossVal.size()==1)
                {

                    c[row][col].setval(c[row][col].PossVal.back());

                    //outfile<<"Set Cells with single possible values"<<c[row][col].giveval()<<"at ("<<row+1<< ","<<col+1<<")"<<endl;

                    cnt++;

                }

            }
        }
        outfile.flush();
        outfile.close();
        return cnt;
    }
/*Solve_num assigns cells with numbers which cannot be assigned to any other cell
 */
    int solve_num()
    {
        ofstream outfile;
        outfile.open("exout2.txt",ios::app);
        for(int row=0;row<9;row++)
        {
            for(int col=0;col<9;col++)
            {

                if(c[row][col].giveval()==0)
                {


                    for(unsigned int i=0;i<c[row][col].PossVal.size();i++)
                    {


                        //row scan

                        int cnt=0;      //No of occurences for that num
                        for(int j=0;j<9;j++)
                        {

                            if(  c[row][j].PossValArray[c[row][col].PossVal[i]]==1 )
                                cnt++;

                        }
                        if(cnt==1)
                        {
                            c[row][col].setval(c[row][col].PossVal[i]);
               //             outfile<<"Setting the value "<<c[row][col].PossVal[i]<<"for("<<row+1<< ","<<col+1<<") using row scan w.r.t num"<<endl;
                            return cnt;
                        }

                        //col scan

                        cnt=0;
                        for(int j=0;j<9;j++)
                        {

                            if(  c[j][col].PossValArray[c[row][col].PossVal[i]]==1 )
                                cnt++;

                        }
                        if(cnt==1)
                        {
                            c[row][col].setval(c[row][col].PossVal[i]);
                    //        outfile<<"Setting the value"<<c[row][col].PossVal[i]<<"for ("<<row+1<< ","<<col+1<<") using col scan w.r.t num"<<endl;
                            return cnt;
                        }



                        //Box scan
                        cnt=0;
                        for(int k=c[row][col].boxrow();k<(c[row][col].boxrow()+3);k++)
                        {
                            for(int j=c[row][col].boxcol();j<(c[row][col].boxcol()+3);j++)
                            {

                                if(  c[k][j].PossValArray[c[row][col].PossVal[i]]==1 )
                                    cnt++;

                            }
                        }
                        if(cnt==1)
                        {
                            c[row][col].setval(c[row][col].PossVal[i]);
                       //      outfile<<"Setting the value"<<c[row][col].PossVal[i]<<"for ("<<row+1<< ","<<col+1<<") using box scan w.r.t num"<<endl;
                            return cnt;
                        }

                    }
                }
            }

        }
        outfile.flush();
        outfile.close();

        return 0;
    }
 
 //Validate the table for consistency
    int validate()
    {
        int digits[10]={0};
        for(int row=0;row<9;row++)
        {
            for(int col=0;col<9;col++)
            {
                if(c[row][col].giveval()==0)
                {
                    if(c[row][col].PossVal.size()==0)
                        return -1;              //Error
                    else
                        return 0;               //Not complete
                }
                digits[c[row][col].giveval()]++;

            }
        }
        for(int i=1;i<10;i++)
        {
            if(digits[i]!=9)
                return -1;
        }


        return 1;           //success


    }
    void minguess(int& minRow,int& minCol )     //find the cell with minimum number of possible values
    {
        unsigned int temp=10;
        for(int row=0;row<8;row++)
        {
            for (int col=0;col<8;col++)
            {
                if(c[row][col].PossVal.size()!=0)
                {
                    if(c[row][col].PossVal.size()<=temp)
                    {
                        temp=c[row][col].PossVal.size();
                        minRow=row;
                        minCol=col;

                    }

                }

            }
        }
    }
    
    void randomize_vector(vector<int> &Possval_rand)
    {
		std::random_shuffle(Possval_rand.begin(),Possval_rand.end());
	}

	void randomize_table()
	{
        srand(time(NULL));
		c[0][0].setval((rand()%9+1));  // Assigning a random initial value
		update_possible_vals(); 
	}
    int solvetable()
    {
		
		
        int i=0;
        int j=0;
        int k;
        do
        {
            do
            {
                i=solve_cell();
                update_possible_vals();

            }while(i>0);

            do
            {

                j=solve_num();
                update_possible_vals();

            }while(j>0);

            if(i==0&&j==0&&validate()==0)
            {
                ofstream outfile;
                outfile.open("exout2.txt",ios::app);
              // outfile<<"**********Entering guessing mode***********************************************"<<endl;


                int row=0;
                int col=0;
                int res,m;
                vector<int> Possval_rand;
                do
                {

                    minguess(row,col);      //Getting the cell which need minimal guesses *For optimisation*
                    m=c[row][col].PossVal.size();
                    for(int gen=0;gen<m;gen++)
                    {
						Possval_rand.push_back(c[row][col].PossVal[gen]);
					}
					randomize_vector(Possval_rand);  //Randomizing the value to be guessed
                    for(unsigned int n=0;n<c[row][col].PossVal.size();n++)
                    {

                        int temp[9][9];
                        for(int e=0;e<9;e++)
                        {
                            for (int f=0;f<9;f++)
                            {
                                temp[e][f]=c[e][f].giveval();

                            }
                        }
                        c[row][col].setval(Possval_rand[n]);
                     //   outfile<<"The guessed value is "<<c[row][col].giveval()<<"at ("<<row+1<<","<<col+1<<")"<<"with size"<<m<<endl;
                        update_possible_vals();
                        res=solvetable();     //Recursive calling of solve table
                        if(res==1)     //Solved the entire table
                            return 1;     
                        else          //Failed to solve the table with the guessed value
                        {
                            for(int e=0;e<9;e++)
                            {
                                for (int f=0;f<9;f++)
                                {
                                    c[e][f].setval(temp[e][f]);   //Reverting back to initial Table

                                }
                            }

                            update_possible_vals();
                        }

                   // outfile<<"The guessed value failed"<<endl;
                    }
                    return -1;
                }while(1);

                outfile.flush();
                outfile.close();
            }
        }while(validate()==0);

        k= validate();
        if(k==-1)                //Failed to solve the table
        {
           return -1;
        }
        if(k==1)
        {
            cout<<"Success you have your answer"<<endl;
            return 1;
        }

	return -1;          //Shouldnt come here. If it comes here then Failure
    }

};

int main()
{
	
    Table t;                    //Instantiate the Table
    t.get_table();              //Populate the Table
    t.randomize_table();		//Randomizing table
    t.show_table();             //Print the Table
    t.solvetable();             //Solve the Table
    t.show_table();             //Print the Table

    
}



