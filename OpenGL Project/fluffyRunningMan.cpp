#include <windows.h>
#include "glwindow.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#define groundTiles 24
#define SPEED 20
#define fallingSpeed 8
#define jumpSpeed 20
#define lowestPos 20
#define playerRelativeSize 0.35
const double PI = 3.1415926535897;
typedef struct
{
   GLfloat x;
   GLfloat y;
   GLfloat z;
   bool isFalling;
   double time;
   double nextStepTime;
   int direction;
   float scale;
   GLfloat defaultColor;
   GLfloat  fallingColor;
   float isAboutToFall;
   bool seen;
   bool hadCollision;
}Pos;
static float playerYTeta = 0;
static float cubeTeta = 0;
static int zMovement = 1;
static int xMovement = 2;
static GLfloat legAngle = 0;
static float gameSpeed = 1;
Pos ground[groundTiles];
CGLWindow window;
GLfloat playerSpeed,PlayerY,playerLatestY;
bool hasPlayerJumped;
double playerTotalTime;
GLfloat cameraAngle;
GLfloat nextCameraAngle;
int spaceLeft;
int movingState;
bool isPlayerFalling,collisionCheck;

void Add_Tile(int a);

//draw player in each frame, Static numbers used have no origin just experience
void DrawPlayer(){
    static int legLoc = 0;
    glRotated(-cameraAngle,0,1,0);
    GLfloat material_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat material_shininess[] = { 10.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
    GLUquadric* quad = gluNewQuadric();
    glColor3f(1,0.1,0);
        glScalef(1,0.5,1);
    gluSphere(quad,playerRelativeSize,20,20);
        glScalef(1,2,1);

    material_diffuse[0]= 1;
    material_diffuse [1] = 1;
    material_diffuse [2] = 1;
    material_diffuse [3] = 0;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_diffuse);

    //legs
	legLoc = 45*sinf((playerYTeta/2)+90);
    glRotated(legLoc,1,0,0);
    glTranslatef(playerRelativeSize/3,-(11*playerRelativeSize)/10,0);
    gluSphere(quad,playerRelativeSize/3,5,20);
    glTranslatef(-playerRelativeSize/3,(11*playerRelativeSize)/10,0);
    glRotated(-legLoc,1,0,0);
    glRotated(-legLoc,1,0,0);
    glTranslatef(-playerRelativeSize/3,-(11*playerRelativeSize)/10,0);
    gluSphere(quad,playerRelativeSize/3,5,20);
    glTranslatef(playerRelativeSize/3,(11*playerRelativeSize)/10,0);
    glRotated(legLoc,1,0,0);

    //arms
    material_diffuse[0]= 1;
    material_diffuse [1] = 1;
    material_diffuse [2] = 1;
    material_diffuse [3] = 0;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_diffuse);
    legLoc = 45*sinf((playerYTeta/2)+90);
    glRotated(legLoc,1,0,0);
    glTranslatef(-(2*playerRelativeSize)/2,-(playerRelativeSize)/3,0);
    gluSphere(quad,playerRelativeSize/2,5,20);
    glTranslatef((2*playerRelativeSize)/2,(playerRelativeSize)/3,0);
    glRotated(-legLoc,1,0,0);
    glRotated(-legLoc,1,0,0);
    glTranslatef((2*playerRelativeSize)/2,-(playerRelativeSize)/3,0);
    gluSphere(quad,playerRelativeSize/2,5,20);
    glTranslatef(-(2*playerRelativeSize)/2,(playerRelativeSize)/3,0);
    glRotated(legLoc,1,0,0);
    glRotated(cameraAngle,0,1,0);
}

//drawing the ground parts, there is probably a better way but I just wanted to see the outcome here
void DrawCube(float defaultColor) {
    GLfloat q = 4;
    GLfloat s = 4;
	GLfloat material_diffuse_s[] = { (1-sinf((cubeTeta/2)))*cosf((PI*defaultColor/2)/groundTiles)*cosf((PI*defaultColor/2)/groundTiles), 2*(1-sinf((cubeTeta/2)))*sinf((PI*defaultColor/2)/groundTiles), 0.0f, 1.0f };
	GLfloat material_specular_s[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat material_shininess_s[] = { 10.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_diffuse_s);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular_s);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess_s);
    GLUquadric* quad = gluNewQuadric();
    glColor3f(1,0.1,0);
    glTranslatef(0,0.5,0.7);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(-1.40,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);

    glTranslatef(0,0.0,-0.35);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    glTranslatef(0.35,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(-1.40,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    glTranslatef(0.35,0.0,0);

    glTranslatef(0,0.0,-0.35);

    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(-1.40,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);

    glTranslatef(0,0.0,-0.35);

    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    glTranslatef(0.35,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(-1.40,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    glTranslatef(0.35,0.0,0);

    glTranslatef(0,0.0,-0.35);

    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(-1.40,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);
    gluSphere(quad,0.05,q,s);
    glTranslatef(0.35,0.0,0);


    glTranslatef(0,-0.5,0.7);
	GLfloat material_diffuse[] = { 0.0f, 0.5f, 0.0f, 1.0f };
	GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat material_shininess[] = { 10.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	glBegin(GL_QUAD_STRIP);
	glColor3f(0,1,0);
	glVertex3f(-1.0f,-0.5f,-1.0f);
	glVertex3f( 1.0f,-0.5f,-1.0f);
	material_diffuse[0] = 0.5*sinf((cubeTeta/2));
	material_diffuse[1] = 0.5*sinf((cubeTeta/2));
    material_diffuse[2] = 0.5*sinf((cubeTeta/2));
	material_shininess[0] = 120.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
	glColor3f(0,1,1);
	glVertex3f(-1.0f, 0.5f,-1.0f);
	glVertex3f( 1.0f, 0.5f,-1.0f);
	glVertex3f(-1.0f, 0.5f, 1.0f);
	glVertex3f( 1.0f, 0.5f, 1.0f);
	material_diffuse[1] = 0.5;
    material_diffuse[2] = 0.0f;
    material_shininess[0] = 10.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
	glColor3f(0,1,0);
	glVertex3f(-1.0f,-0.5f, 1.0f);
	glVertex3f( 1.0f,-0.5f, 1.0f);
	glColor3f(0,1,0);
	glVertex3f(-1.0f,-0.5f,1.0f);
	glVertex3f( 1.0f,-0.5f,1.0f);
	glEnd();

	glColor3f(0,1,0);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f,-0.5f,-1.0f);
	glVertex3f(-1.0f, 0.5f,-1.0f);
	glVertex3f(-1.0f, 0.5f, 1.0f);
	glVertex3f(-1.0f,-0.5f, 1.0f);

	glColor3f(0,1,0);
	glVertex3f( 1.0f,-0.5f,-1.0f);
	glVertex3f( 1.0f, 0.5f,-1.0f);
	glVertex3f( 1.0f, 0.5f, 1.0f);
	glVertex3f( 1.0f,-0.5f, 1.0f);
	glEnd();
}

//creating the initil space with 10 falling cubes
void Initialize() {
    srand (time(NULL));
        ground[0].isFalling= false;
        ground[0].z = -16.0f;
        ground[0].x = 0.0f;
        ground[0].y = 0.0f;
        ground[0].time = 0;
        ground[0].nextStepTime = 0;
        ground[0].direction = 0;
        ground[0].scale = 2;
        ground[0].isAboutToFall = 0;
    for(int a = 1;a<11;a++){
        ground[a].isFalling= false;
        ground[a].z = 4.0f;
        ground[a].x = 0.0f;
        ground[a].y = 0.0f;
        ground[a].time = 0;
        ground[a].nextStepTime = 0;
        ground[a].direction = 0;
        ground[a].scale = 2;
        ground[a].isAboutToFall = a;
        ground[a].seen = true;
        ground[a].hadCollision = false;
    }
    for(int i = 12;i<groundTiles;i++){
        Add_Tile(i);
        ground[i-1].defaultColor = 0.5f;
        ground[i-1].isAboutToFall = i;
        ground[i-1].isFalling= false;
        ground[i-1].scale = 2;
    }
    gameSpeed = 1;
    spaceLeft = 2;
    isPlayerFalling = false;
    movingState = 0;
    playerSpeed = 0;
    hasPlayerJumped = false;
    PlayerY = 1.25 + 2*playerRelativeSize/3;
    playerTotalTime = 0;
    cameraAngle = 0;
    GLfloat light_position[] = { 0.0f, 1.0f, 0.0f, 0.05f };
	GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);


//     glutKeyboardFunc(CameraMan2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, 1.3f, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

//Rendering the world
void Render() {
	float alpha = GetTickCount()/10.0f;
	float scale = sinf(GetTickCount()/1000.0f)+1.1f;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	if((cameraAngle < nextCameraAngle)){
        cameraAngle += 5;
	}else if((cameraAngle > nextCameraAngle)){
        cameraAngle -= 5;
	}
    glRotated(cameraAngle,0,1,0);
    glPushMatrix();
    playerLatestY = PlayerY;
	glTranslatef(0,playerLatestY,0);
	DrawPlayer();
	glTranslatef(0,-(playerLatestY),0);
    glTranslatef(-(ground[0].x),-(ground[0].y),-(ground[0].z));
	for(int i = 1;i<groundTiles;i++){
        playerLatestY = ground[i].y;
        glTranslatef(-(ground[i].x),-(playerLatestY),-(ground[i].z));
        glPushMatrix();
        glScalef(ground[i].scale-2*sinf(ground[i].time),ground[i].scale-2*sinf(ground[i].time),ground[i].scale-2*sinf(ground[i].time));
            if(ground[i].seen){
              DrawCube(ground[i].isAboutToFall);
        }
        glPopMatrix();
        glTranslatef(0,(playerLatestY),0);
	}
    glPopMatrix();

}

//Moving functions in different directions
void Move_Z(int j){

        if(j==1){
            ground[0].z -= 0.1;
        }else if (j==0){
            ground[0].z += 0.1;
        }
}
void Move_X(int j){

        if(j==1){
            ground[0].x -= 0.1;
        }else if (j ==0){
            ground[0].x += 0.1;
        }

}
//replacing the fallen tiles
void Add_Tile(int a){
    int i = rand()%4;
    if(i==((ground[a-2].direction + 2)%4)){
        i = ground[a-2].direction;
        if (((rand()%3)!=0)&&(ground[a-3].isFalling==false)&&ground[a-2].hadCollision==false)
            ground[a-2].isFalling= true;
    }
    ground[a-1].direction = i;
    ground[a-1].isFalling= false;
    ground[a-1].time = PI/2;
    ground[a-1].nextStepTime = 0;
    ground[a-1].hadCollision = false;
    ground[a-1].y = 0;
    ground[a-1].seen = true;
    ground[a-1].scale = 2;
    ground[a-1].isAboutToFall = a-1;
    switch(i){
    case 0:
        ground[a-1].z = 4.0f;
        ground[a-1].x = 0.0f;
        break;
    case 2:
        ground[a-1].z = -4.0f;
        ground[a-1].x = 0.0f;
        break;
    case 1:
        ground[a-1].z = 0.0f;
        ground[a-1].x = -4.0f;
        break;
    case 3:
        ground[a-1].z = 0.0f;
        ground[a-1].x = 4.0f;
        break;
    }
    GLfloat x = 0,z = 0;
    for(int i = a-1;i>0;i--){
        x += ground[i].x;
        z += ground[i].z;
        if(x==0 && z==0 && ground[i-1].seen == true && ground[i-1].isFalling== false ){
            ground[i-1].seen = false;
            ground[a-1].hadCollision = true;
            return;
        }
    }

}
//Dropping out of time tiles
void DROP_tile(){
    for(int i = 1;i<(groundTiles/7);i++){
        if(ground[i].isFalling== false){
            ground[i].isFalling= true;
            break;
        }
    }
    if(ground[1].y > lowestPos){
            ground[0].x += ground[1].x;
            ground[0].z += ground[1].z;
        for(int i = 1;i<(groundTiles-1);i++){
            ground[i].isFalling= ground[i+1].isFalling;
            ground[i].x = ground[i+1].x;
            ground[i].y = ground[i+1].y;
            ground[i].z = ground[i+1].z;
            ground[i].time = ground[i+1].time;
            ground[i].nextStepTime = ground[i+1].nextStepTime;
            ground[i].direction = ground[i+1].direction;
            ground[i].seen = ground[i+1].seen;
            ground[i].hadCollision = ground[i+1].hadCollision;
        }
        Add_Tile(groundTiles);
    }
}

//Creating a real life speed for tiles falling
void Fall(){
    int i = 1;
    while((i<groundTiles)){
        if(ground[i].isFalling== true){
            ground[i].nextStepTime += 0.001*fallingSpeed;
            ground[i].y += 10*ground[i].nextStepTime*ground[i].nextStepTime;
        }
        i++;
    }
}

//making sure basic physical laws are met
void Check_Physics(){
    GLfloat x = ground[0].x ;
    GLfloat z = ground[0].z;
    bool isFalling= true;
    for(int i = 1;i<(groundTiles);i++){
        //just using the loop :D
        if(ground[i].time>0){
            ground[i].time -= PI/20;
        }else{
            ground[i].time = 0;
        }
        ////////
        x += ground[i].x;
        z += ground[i].z;
        if((x<2+playerRelativeSize)&&(x>-2-playerRelativeSize)&&(z<2+playerRelativeSize)&&(z>-2-playerRelativeSize)){
        if(((ground[i].y-PlayerY)>-(playerRelativeSize+0.80))&&((ground[i].y-PlayerY)<(playerRelativeSize+0.90))&&(ground[i].isFalling==false)){
                    if((playerSpeed>0)&&(x<2)&&(x>-2)&&(z<2)&&(z>-2)){
                        playerSpeed = 0;
                    }else{
                    collisionCheck = false;
                    }
            }
        if((x<2)&&(x>-2)&&(z<2)&&(z>-2)){
            if((PlayerY>1)&&(ground[i].isFalling==false)){
                isFalling= false;
                isPlayerFalling = false;
                collisionCheck = true;
            }
        }
        }
    }
    if(isFalling){
            isPlayerFalling = true;
            hasPlayerJumped = true;
        }
}

//this is where the magic happens
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static float up = 0;
    static float cntrX = 0, cntrY = 0, cntrZ = 0;
    static float eyeLat = PI/4, eyeLon = 0, eyeR = 6 ;
    static int Count_Down = 1000/SPEED;
    static bool finished = false;
    static char buff[100];
    static int score = 0;
	switch(uMsg) {
        case WM_TIMER:
            window.Swap();
            //Player speeds up
            gameSpeed =+ 0.001;
            //Checking if the player has fallen
            if((PlayerY<-30)&&finished == false){
                collisionCheck = false;
                finished = true;
                sprintf(buff, "I hope you enjoyed the game!\n\nYour Score: %d\n\nCreated by ATK",score);
                MessageBox(NULL,buff, "You Died!", MB_OK | MB_ICONQUESTION);
                PostQuitMessage(0);
                window.Terminate();

            }
            Count_Down--;
            playerYTeta += PI/6.; if(playerYTeta >=  4*PI) {
                playerYTeta =  0;
                score++;

            }
            playerTotalTime += PI/32.; if(playerTotalTime >=  2*PI) {
                playerTotalTime =  0;
            }
            cubeTeta += PI/32.; if(cubeTeta >=  4*PI) {
                cubeTeta =  0;
            }

            PlayerY += 0.02*sinf(playerYTeta);
            if(hasPlayerJumped){
                PlayerY += playerSpeed*0.001*jumpSpeed;
                playerSpeed -= 5*0.004*jumpSpeed;
                if ((PlayerY<1.5 + 2*playerRelativeSize/3)&&(playerSpeed<0)&&(!isPlayerFalling)){
                    hasPlayerJumped = false;
                    playerSpeed = 0;
                    spaceLeft = 2;
                    PlayerY = 1.5 + 2*playerRelativeSize/3;
                    playerYTeta = 3*PI/2;
                }
            }
            Check_Physics();

            if(Count_Down < 1){
                DROP_tile();
                Count_Down = 1000/SPEED;
            }
            glLoadIdentity();
            Fall();
            if(collisionCheck){
                Move_X(xMovement);
                Move_Z(zMovement);
            }
            gluLookAt(
                (eyeR * sinf(eyeLon) * cosf(eyeLat)),
                eyeR * sinf(eyeLat),
                (eyeR * cosf(eyeLon) * cosf(eyeLat)),
                cntrX, cntrY+1, cntrZ-3,
                sinf(up), cosf(up), 0);

            Render();
            break;

        //changing direction or camera angle or making a jump
		case WM_KEYDOWN:
			switch(wParam) {
				case VK_ESCAPE: PostQuitMessage(0); break;

                case VK_LEFT:  eyeLon -= PI/32.; break;
                case VK_RIGHT: eyeLon += PI/32.; break;
                case VK_UP:    eyeLat += PI/32.; if(eyeLat >=  PI/2) eyeLat =  15*PI/32; break;
                case VK_DOWN:  eyeLat -= PI/32.; if(eyeLat <= -PI/2) eyeLat = -15*PI/32; break;
                case 'r': case 'R':  cntrX = 0; cntrY = 0; cntrZ = 0;
                eyeLat = PI/4; eyeLon = 0; eyeR = 6;   break;
                case 'w': case 'W':  eyeR -= 0.5;   break;
                case 's': case 'S': eyeR += 0.5; break;
                case 'D': case 'd':
                    if(movingState == 0){
                    zMovement = 2;xMovement = 0;
                    movingState = 1;
                    }else if(movingState ==1){
                    zMovement = 0;xMovement = 2;
                    movingState = 2;
                    }else if(movingState ==2){
                    zMovement = 2;xMovement = 1;
                    movingState = 3;
                    }else if(movingState ==3){
                    zMovement = 1;xMovement = 2;
                    movingState = 0;
                    }
                    nextCameraAngle += 90;
                     break;

                case 'A': case 'a':
                    if(movingState == 0){
                    zMovement = 2;xMovement = 1;
                    movingState = 3;
                    }else if(movingState ==1){
                    zMovement = 1;xMovement = 2;
                    movingState = 0;
                    }else if(movingState ==2){
                    zMovement = 2;xMovement = 0;
                    movingState = 1;
                    }else if(movingState ==3){
                    zMovement = 0;xMovement = 2;
                    movingState = 2;
                    }
                    nextCameraAngle -= 90;
                     break;
                case VK_SPACE:
                    if(spaceLeft>0){
                    hasPlayerJumped = true;
                    playerSpeed = 8;
                    spaceLeft--;
                }
                    break;


				default:
					break;
			}
			break;
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			window.Resize(LOWORD(lParam), HIWORD(lParam));
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow) {

	MSG msg;

    MessageBox(NULL, "Hello!\nThis is a Survival Mode Game!\nWhich means you have to stay on the cubes as long as you can!\nGreen-Red lights on each cube surface show how dangerous it is for you to stand on it! Red ones are going to fall!\nKeys:\nA -> Turn Left\nD -> Turn Right\nSpace -> Jump (You can jump twice without landing)\nW -> Reduces distance between Camera and The Little Guy\nS -> Increases distance between Camera and The Little Guy\nYou can also use the Arrow Keys to turn camera around The Little Guy!\nFinally pressing \'R\' will reset camera position!\nHave fun!", "Tutorial", MB_OK | MB_ICONQUESTION);

	window.Initialize(800, 600, 32, false);

	Initialize();
    SetTimer(window.hWnd, 1, 10, NULL);

    // Run the message loop. It will run until GetMessage() returns 0 
    while (GetMessage (&msg, NULL, 0, 0))
    {
        // Translate virtual-key messages into character messages 
        TranslateMessage(&msg);
        // Send message to WindowProcedure 
        DispatchMessage(&msg);
    }

	window.Terminate();

    // The program return-value is 0 - The value that PostQuitMessage() gave 

	return msg.wParam;
}
