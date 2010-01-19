/* William Reinhardt
   CS211
   Homework 1 */

#include <stdio.h>
#include <math.h>
#include <time.h>

/* Function prototypes */
void gameIntro(char playerName[]);
void printDirections();
int gamePlay(char playerName[], double compShot[]);
int randomNumber(int min,int max);
double deg2rad(int angle);
double calcProjDist(double angle, double velocity);
void getCompVelocity(double a[]);
void computerPlay(double a[],int turns);
int playAgain();
void talkSmack(double average);

/* Declare global constants */
#define ANGLE 0
#define VEL 1
#define COMPSHOT 2
#define DIST 3
#define OVERSHOT 4
#define UNDERSHOT 5

int main()
{
    /* Declare pointers and variables */
    char playerName[10];
    int wins = 0, games = 0;
    int another = 1;
    double compShot[6];

    srand(time(0));

    gameIntro(playerName);

    while (another)
    {
    	games++;
    	wins = wins + gamePlay(playerName, compShot);
    	another = playAgain();
    }

    printf("You won %d out of %d games.\n",wins,games);
    talkSmack((double)wins/games);
    printf("Good bye!\n\n");

    return 0;
}

void gameIntro(char playerName[])
{
    /* Declare variables */
    char c;

    /* Welcome player */
    printf("************************************\n");
    printf("*****   WELCOME TO ARTILLERY!  *****\n");
    printf("************************************\n\n");
    printf("Please enter your name: ");
    scanf("%s",playerName);

    /* Ask player if he'd like directions */
    printf("%s, would you like directions on how to play the game? (y/n): "
	   ,playerName);
    scanf("%c",&c);
    scanf("%c",&c);
    if ( c == 'y' )
	printDirections();
}

void printDirections()
{
    printf("\nYour objective is to destroy the computer opponent.\n");
    printf("To do this, you will need to enter in an angle and velocity\n");
    printf("at which you wish to fire your cannon.\n\n");
    printf("You and the computer will take turns firing at each other\n");
    printf("until one of you has fired a projectile within 5 meters of\n");
    printf("the opponent's cannon.\n\n");
}

int gamePlay(char playerName[], double compShot[])
{
    /* Declare variables */
    int distance, velocity, angle, result = 0, turns = 0;
    double projDist, radian;
    int done = 0;

    /* Generate random distance between 100 and 200 */
    distance = randomNumber(100,200);
    compShot[DIST] = distance;
    printf("%s, the distance between you and the computer is %d meters.\n"
	   ,playerName,distance);
    compShot[ANGLE] = randomNumber(5,90);
    if ( compShot[ANGLE] > 26 && compShot[ANGLE] < 64)
    	compShot[VEL] = randomNumber(49,55);
    else
    	compShot[VEL] = randomNumber(110,115);

    while(!done)
    {
    	printf("Enter the angle at which you wish to fire: ");
    	scanf("%d",&angle);
    	radian = deg2rad(angle);
    	printf("Enter the velocity at which you wish to fire: ");
    	scanf("%d",&velocity);

    	/* Assign return of calcProjDist to projDist */
    	projDist = calcProjDist(radian,(double) velocity);
    	printf("You fire the missle %.2lf meters.\n",projDist);

    	/* Check to see if player won */
    	if ( projDist > distance - 5 && projDist < distance + 5 )
    	{
    		printf("*****   YOU WIN!!!   *****\n");
    		done = 1;
    		result = 1;
    	}
    	else /* Computer player's turn */
    	{
    		computerPlay(compShot,turns); /*Call computerPlay function */
    		if (turns == 0)
    		{
    			compShot[UNDERSHOT] = 0;
    			compShot[OVERSHOT] = 0;
    		}
    		printf("The computer adjusts its velocity to %.2lf meters/second.\n"
    				,compShot[VEL]);

    		getCompVelocity(compShot); /* Adjust computer velocity */
    		printf("The computer fires its missle %.2lf meters.\n",
    				compShot[COMPSHOT]);
    		turns++;

    		/* Check to see if computer won */
    		if (compShot[COMPSHOT] > distance - 5
    				&& compShot[COMPSHOT] < distance + 5)
    		{
    			printf("*****  YOU LOSE!!!   *****\n");
    			done = 1;
				}
		}
    }

    return result;
}

/* Generate a random number */
int randomNumber(int min,int max)
{
    return rand()%(max-min+1)+min;
}

/* Convert degrees to radians */
double deg2rad(int angle)
{
    return M_PI * angle / 180.0;
}

/* Calculate the distance the projectile travels */
double calcProjDist(double angle, double velocity)
{
    return velocity * velocity * sin( 2 * angle ) / 9.8;
}

void getCompVelocity(double a[])
{
    if ( a[COMPSHOT] > a[DIST] )
    {
    	a[OVERSHOT] = a[VEL];
    	a[VEL] = (a[OVERSHOT] + a[UNDERSHOT]) / 2;
    }

    if ( a[COMPSHOT] < a[DIST] )
    {
    	a[UNDERSHOT] = a[VEL];
		a[VEL] = (a[OVERSHOT] + a[UNDERSHOT]) / 2;
    }
}

void computerPlay(double a[],int turns)
{
    double radians, velocity;

    radians = a[ANGLE];
    radians = deg2rad(radians);
    a[COMPSHOT] = calcProjDist(radians, a[VEL]);
}

/* Find out if player would like another game */
int playAgain()
{
    char c;

    printf("Would you like to play again? (y/n): ");
    scanf("%c",&c);
    scanf("%c",&c);

    if (c == 'y')
    	return 1;

    return 0;
}

/* Tell player how he faired */
void talkSmack(double average)
{
    double per;

    per = average * 100;
    printf("Your average was %.2lf%%.\n",per);

    if ( average > .89 )
    	printf("Great job! Go join the army!\n");
    else if ( average <= .89 && average > .59 )
    	printf("Meh, you did OK.\n");
    else
    	printf("You have my pity.\n");
}
