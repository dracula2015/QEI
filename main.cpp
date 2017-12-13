#include"openglshowlist.h"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

GLuint robot,wheel;
GLUquadricObj *disk;
GLuint texture;
GLuint front;
GLuint back;
GLuint top;
GLuint bottom;
GLuint left;
GLuint right;

Vector3f qd_pre(0.8, 0, 0);
Vector3f dqd_pre(0, 0.1676, 0);
Vector3f dq_pre(0, 0, 0);
Vector3f dq = dq_pre;
Vector3f q_pre(0, 0, 0);
Vector3f q = q_pre;
Vector3f qd;
Vector3f dqd;
Vector3f ddqd;
Vector3f ddq;
Vector3f u;


POINT pos,originalPos;
long mouseWheel;
bool bMoveForward = false;
bool bMoveBackward = false;
bool bMoveLeft = false;
bool bMoveRight = false;
bool bRotate = false;


//Vector3f cameraPos(0, 0, 0);
//Vector3f cameraView(0, 0, -1.0f);
//Vector3f cameraUp(0, 1.0f, 0);

Vector3f cameraPos(0, 3000, 0);
Vector3f cameraView(0, 0, 0);
Vector3f cameraUp(0, 0, -1.0f);

Vector3f shift(0, 0, 0);
Vector3f spin_wheel(0, 0, 0);
static float t = 0;
float spin_body = 0;
//dos修改
float wheel_shift = 240;
float speed = 20.0f;
float amplitudeA =2, amplitudeB =2;
float dt = 0.01;

void RotateView(float angle, float x, float y, float z)
{
	Vector3f cameraDirection;
	Vector3f newCameraDirection;
	float c = cos(angle);
	float s = sin(angle);
	cameraDirection = cameraView - cameraPos;
	Vector3f tempX = Vector3f(c + x*x*(1 - c), x*y*(1 - c) - z*s, x*z*(1 - c) + y*s);
	Vector3f tempY = Vector3f(x*y*(1 - c) + z*s, c + y*y*(1 - c), y*z*(1 - c) - x*s);
	Vector3f tempZ = Vector3f(x*z*(1 - c) - y*s, y*z*(1 - c) + x*s, c + z*z*(1 - c));

	newCameraDirection.x = cameraDirection*tempX;
	newCameraDirection.y = cameraDirection*tempY;
	newCameraDirection.z = cameraDirection*tempZ;

	cameraView = cameraPos + newCameraDirection;

}

void RotateUp(float angle, float x, float y, float z)
{
	float c = cos(angle);
	float s = sin(angle);
	Vector3f temp = cameraUp;
	Vector3f tempX = Vector3f(c + x*x*(1 - c), x*y*(1 - c) - z*s, x*z*(1 - c) + y*s);
	Vector3f tempY = Vector3f(x*y*(1 - c) + z*s, c + y*y*(1 - c), y*z*(1 - c) - x*s);
	Vector3f tempZ = Vector3f(x*z*(1 - c) - y*s, y*z*(1 - c) + x*s, c + z*z*(1 - c));

	cameraUp.x = temp*tempX;
	cameraUp.y = temp*tempY;
	cameraUp.z = temp*tempZ;

}


unsigned char* loadBMP(const char* path, int &width, int &height) 
{
	unsigned char* imageData = NULL;
	FILE*pFile = fopen(path, "rb");
	if (pFile) 
	{
		BITMAPFILEHEADER bfh;
		fread(&bfh, sizeof(BITMAPFILEHEADER), 1, pFile);
		if(bfh.bfType==0x4D42)
		{
			BITMAPINFOHEADER bih;
			fread(&bih, sizeof(BITMAPINFOHEADER), 1, pFile);
			width = bih.biWidth;
			height = bih.biHeight;
			imageData = new unsigned char[width*height * 3];
			fseek(pFile, bfh.bfOffBits, SEEK_SET);
			fread(imageData, 1, width*height * 3, pFile);
			fclose(pFile);
		}
	}
	return imageData;
}

GLuint CreateTexture(const char* path) {
	int imageWidth, imageHeight;
	unsigned char*imageData = loadBMP(path, imageWidth, imageHeight);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glBindTexture(GL_TEXTURE_2D,0);
	return texture;
}

void DrawScene()
{
	spin_wheel = u*100;//驱动轮转角
	shift = q*1000;//机体位姿
	spin_body = shift.z*0.0572958;/*2*pi/360=0.0572958*/
	float a = 2000.0f, b = 0.0f;//镜面位置及大小
	//结构性轮偏移
	//float wheel_shift = 2;
	//光照
	float ambientLight[] = { 1.0f,1.0f,1.0f,1.0f };
	float diffuseLight[] = { 1.0f,1.0f,1.0f,1.0f };
	float specuarLight[] = { 1.0f,1.0f,1.0f,1.0f };
	float position[] = { 0, 0.0f, 5.0f, 0.0f };
	float direction[] = { 1.0f,1.0f,-4.0f,1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specuarLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01f);
	float ambientMaterial[] = { 0.1f,0,0,1.0f };
	float diffuseMaterial[] = { 1.0f,0,0,1.0f };
	float specularMaterial[] = { 1.0f,0.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//天空盒
	glPushMatrix();
	glTranslatef(cameraPos.x,cameraPos.y,cameraPos.z);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//front
	glBindTexture(GL_TEXTURE_2D, front);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0, 0);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(0, 1);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(1, 1);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glEnd();

	//back
	glBindTexture(GL_TEXTURE_2D, back);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(0, 1);
	glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(1, 1);
	glVertex3f(0.5, 0.5, 0.5);
	glEnd();

	//left
	glBindTexture(GL_TEXTURE_2D, left);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(0, 1);
	glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(1, 1);
	glVertex3f(-0.5, 0.5, 0.5);
	glEnd();

	//right
	glBindTexture(GL_TEXTURE_2D, right);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(0, 0);
	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(0, 1);
	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(1, 1);
	glVertex3f(0.5, 0.5, -0.5);
	glEnd();

	//top
	glBindTexture(GL_TEXTURE_2D, top);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(1, 1);
	glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2f(1, 0);
	glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5, 0.5, 0.5);
	glEnd();

	//bottom
	glBindTexture(GL_TEXTURE_2D, bottom);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(1, 0);
	glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(0, 0);
	glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(0, 1);
	glVertex3f(-0.5, -0.5, 0.5); 
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//球，镜面，镜像
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	//镜面
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-a, b, a);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(a, b, a);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(a, b, -a);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-a, b, -a);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	//镜像
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glPushMatrix();
	glTranslatef(0, -100.0f,0);
	glEnable(GL_LIGHTING);
	gluSphere(disk, 50.0f, 360, 20);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	//球
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glPushMatrix();
	glEnable(GL_LIGHTING);
	gluSphere(disk, 50.0f, 360, 20);

	float xx=0, yy=0;
	for (int i = 0; i < 360; i += 10) {
		xx = 1000*amplitudeA*cos(3.14 / 180 * i);
		yy = 1000*amplitudeB*sin(3.14 / 180 * i);
		glPushMatrix();
		glTranslatef(xx, 0, yy);
		gluSphere(disk, 50.0f, 360, 20);
		glPopMatrix();
	}

	glDisable(GL_LIGHTING);
	//gluDisk(disk, 5.0f, 10.0f, 40, 5);
	//gluCylinder(disk, 10.0f, 5.0f, 5.0f, 20, 10);
	glPopMatrix();

	//机器人
	glPushMatrix();
	glTranslatef(shift.x, 0, shift.y);
	glRotatef(spin_body,0,1,0);
	//轴一
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslatef(0, 0, wheel_shift);
	glRotatef(spin_wheel.x, 0, 0, 1);
	glEnable(GL_LIGHTING);
	glCallList(wheel);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	//轴二
	glPushMatrix();
	glRotated(120, 0, 1, 0);
	glTranslatef(0, 0, wheel_shift);
	glRotatef(spin_wheel.y, 0, 0, 1);
	glEnable(GL_LIGHTING);
	glCallList(wheel);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	//轴三
	glPushMatrix();
	glRotated(240, 0, 1, 0);
	glTranslatef(0, 0, wheel_shift);
	glRotatef(spin_wheel.z, 0, 0, 1);
	glEnable(GL_LIGHTING);
	glCallList(wheel);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	//机体
	glPushMatrix();
	glTranslatef(0, 229.5f, 0);
	//glRotated(-90, 1, 0, 0);
	//glScalef(5, 5, 5);
	glEnable(GL_LIGHTING);
	glCallList(robot);
	glDisable(GL_LIGHTING);
	glPopMatrix();
	//机器人
	glPopMatrix();

	glLoadIdentity();

}

LRESULT CALLBACK RenderWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT currentPos;
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case 'W':
			bMoveForward = true;
			break;
		case 'S':
			bMoveBackward = true;
			break;
		case 'D':
			bMoveRight = true;
			break;
		case 'A':
			bMoveLeft = true;
			break;
		case 'P':
			printf("please input the parameters x(default x=2): ");
			scanf("%f", &amplitudeA);
			printf("please input the parameters y(default y=2): ");
			scanf("%f", &amplitudeB);
			printf("please input the parameters dt(default dt=0.01): ");
			scanf("%f", &dt);
			//printf("please input the parameters wheel_shift: ");
			//scanf("%f", &wheel_shift);
			printf("please input the parameters camera speed(default camera speed=20): ");
			scanf("%f", &speed);
			break;
		}
		break;
	case WM_KEYUP:
		bMoveForward = false;
		bMoveBackward = false;
		bMoveRight = false;
		bMoveLeft = false;
		break;
	case WM_RBUTTONDOWN:
		bRotate = true;
		pos.x = LOWORD(lParam);
		pos.y = HIWORD(lParam);
		originalPos.x = pos.x;
		originalPos.y = pos.y;
		ClientToScreen(hwnd, &originalPos);
		SetCapture(hwnd);
		ShowCursor(false);

		break;
	case WM_LBUTTONDOWN:
	{int xpos = LOWORD(lParam);
	int ypos = HIWORD(lParam);
	printf("%ld %ld \n", xpos, ypos);
	break; }
	case WM_RBUTTONUP:
		bRotate = false;
		ShowCursor(true);
		SetCursorPos(originalPos.x, originalPos.y);
		ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		if (bRotate)
		{
			currentPos.x = LOWORD(lParam);
			currentPos.y = HIWORD(lParam);
			int deltaX = pos.x - currentPos.x;
			int deltaY = pos.y - currentPos.y;
			float angleY = deltaX / 1000.0f;
			float angleX = deltaY / 1000.0f;
			//printf("%f \n", angleX);
			//scanf();
			Vector3f temp = cameraView - cameraPos;
			temp = temp ^ cameraUp;
			temp.Normalize();
			RotateView(angleY,cameraUp.x,cameraUp.y,cameraUp.z);
			RotateView(angleX,temp.x,temp.y,temp.z);
			RotateUp(angleX, temp.x, temp.y, temp.z);
			//printf("%f%f%f->%f%f%f \n", cameraPos.x, cameraPos.y, cameraPos.z, cameraView.x, cameraView.y, cameraView.z);
			SetCursorPos(originalPos.x, originalPos.y);
			pos.x = originalPos.x;
			pos.y = originalPos.y;
			ScreenToClient(hwnd, &pos);
		}
		break;
	case WM_MOUSEWHEEL:
		mouseWheel = HIWORD(wParam);
		if(65536-mouseWheel<32768)
		{
			Vector3f cameraDirection = cameraView - cameraPos;
			cameraDirection.Normalize();
			cameraPos = cameraPos - cameraDirection*speed*5.0f;
			cameraView = cameraView - cameraDirection*speed*5.0f;

		}else if(65536-mouseWheel>32768)
		{
			Vector3f cameraDirection = cameraView - cameraPos;
			cameraDirection.Normalize();
			cameraPos = cameraPos + cameraDirection*speed*5.0f;
			cameraView = cameraView + cameraDirection*speed*5.0f;
		}		
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = NULL;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = NULL;
	wndClass.hIconSm = NULL;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = RenderWindowProc;
	wndClass.lpszClassName = L"OpenGLWindow";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	ATOM atom = RegisterClassEx(&wndClass);

	HWND hwnd = CreateWindowEx(NULL, L"OpenGLWindow", L"OpenGl Render Window", WS_OVERLAPPEDWINDOW, 100, 100, 3200, 2400, NULL, NULL, hInstance, NULL);
	
	HDC dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	int pixelFormatID = ChoosePixelFormat(dc, &pfd);
	
	SetPixelFormat(dc, pixelFormatID, &pfd);

	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);

	glEnable(GL_LIGHT0);
	float ambientLight[] = { 1.0f,1.0f,1.0f,1.0f };
	float diffuseLight[] = { 1.0f,1.0f,1.0f,1.0f };
	float specuarLight[] = { 1.0f,1.0f,1.0f,1.0f };
	float position[]  ={ 0, 10.0f, 0.0f, 0.0f };
	float direction[] = { 1.0f,1.0f,-4.0f,1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specuarLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01f);
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 65.0f);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0f);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);

	float ambientMaterial[] = { 0.1f,0,0,1.0f };
	float diffuseMaterial[] = { 1.0f,0,0,1.0f };
	float specularMaterial[] = { 1.0f,0.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1.0f);
	glEnable(GL_LIGHTING);

	disk = gluNewQuadric();
	robot = Gen3DObjectList_0();
	wheel = Gen3DObjectList_1();
	
	glClearColor(41.0 / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0f, 3200.0f / 2400.0f, 0.1f, 100000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	MSG msg;

	clock_t start, end;
	texture = CreateTexture("test.bmp");
	front = CreateTexture("front.bmp");
	back = CreateTexture("back.bmp");
	top = CreateTexture("top.bmp");
	bottom = CreateTexture("bottom.bmp");
	left = CreateTexture("left.bmp");
	right = CreateTexture("right.bmp");

	while (true)
	{
		start = clock();
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;

			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		glClear(GL_COLOR_BUFFER_BIT);
		if (bMoveForward) 
		{
		/*	Vector3f cameraDirection = cameraView - cameraPos;
			cameraDirection.Normalize();
			cameraPos = cameraPos + cameraDirection*speed*1.0f;
			cameraView = cameraView + cameraDirection*speed*1.0f;*/
			cameraPos = cameraPos + cameraUp*speed*1.0f;
			cameraView = cameraView + cameraUp*speed*1.0f;

		}
		if (bMoveBackward) 
		{
			/*Vector3f cameraDirection = cameraView - cameraPos;
			cameraDirection.Normalize();
			cameraPos = cameraPos - cameraDirection*speed*1.0f;
			cameraView = cameraView - cameraDirection*speed*1.0f;*/
			cameraPos = cameraPos - cameraUp*speed*1.0f;
			cameraView = cameraView - cameraUp*speed*1.0f;
		}
		if(bMoveLeft)
		{
			Vector3f temp = cameraView - cameraPos;
			temp = cameraUp^temp;
			temp.Normalize();
			cameraPos = cameraPos + temp*speed*1.0f;
			cameraView = cameraView + temp*speed*1.0f;
		}

		if (bMoveRight) 
		{
			Vector3f temp = cameraView - cameraPos;
			temp = cameraUp^temp;
			temp.Normalize();
			cameraPos = cameraPos - temp*speed*1.0f;
			cameraView = cameraView - temp*speed*1.0f;

		}
		glLoadIdentity();
		gluLookAt(cameraPos.x,cameraPos.y,cameraPos.z,cameraView.x,cameraView.y,cameraView.z,cameraUp.x,cameraUp.y,cameraUp.z);	
		float x = amplitudeA*cos(3.14 / 15 * t);
		float y = amplitudeB*sin(3.14 / 15 * t);
		float theta;
		if(t>10)
		{ 
			theta = 3.5*(t - 10); 
		}	
		else 
		{ 
			theta = 0; 
		}

		qd = Vector3f(x, y, theta);
		dqd = (qd - qd_pre)*(1/dt);
		qd_pre = qd;
		ddqd = (dqd - dqd_pre)*(1/dt);
		dqd_pre = dqd;
		u = OMRS_controller(qd, dqd, ddqd, q, dq);
		ddq = OMRS_model(u, q, dq);
		dq = dq_pre + ddq*dt;
		dq_pre = dq;
		q = q_pre + dq*dt;
		q_pre = q;

		t += dt;
		DrawScene();
		SwapBuffers(dc);
		end = clock();
		//Matrix a(8,5,4,9,2,7,3,1,5);
		//printf("%f %f %f\n %f %f %f\n %f %f %f\n ", a.Inverse().triMatrix[0][0], a.Inverse().triMatrix[0][1],a.Inverse().triMatrix[0][2],a.Inverse().triMatrix[1][0],a.Inverse().triMatrix[1][1],a.Inverse().triMatrix[1][2],a.Inverse().triMatrix[2][0],a.Inverse().triMatrix[2][1],a.Inverse().triMatrix[2][2]);
		//printf("%f %f %f \n %f %f %f %f\n", q.x, q.y, q.z,x,y,t,dt);
		//printf("一个主循环的时间：%lf \n 开始时间： %ld\n 结束时间： %ld\n 每秒周期数：%ld\n",(end-start)/ CLOCKS_PER_SEC, start / CLOCKS_PER_SEC,end / CLOCKS_PER_SEC,CLOCKS_PER_SEC);

	}
	
	return 0;
}
