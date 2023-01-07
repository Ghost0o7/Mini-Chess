#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <GL/glut.h>
#include <GL/gl.h>

#include <time.h>
#include <math.h>


#define M_PI 3.14159265358979323846
const int boardSize = 8; // Size of the chess board
const int squareSize = 5; // Size of each square on the board
const int numVertices = 360; // Number of vertices to use for the circle

int whitePawnPositions[3][2]; // Positions of the white pawns
int blackPawnPositions[3][2]; // Positions of the black pawns

int previousWhitePawnPositions[3][2]; // Previous positions of the white pawns
int previousBlackPawnPositions[3][2]; // Previous positions of the black pawns

int whitePawnsRemaining = 3;
int whitePawnsAtEnd = 0;
int blackPawnsAtEnd = 0;
int blackPawnsRemaining = 3;

int whiteBack=3;
int blackBack=3;


bool pawnsSpawned = false; // Flag to track whether the pawns have already been spawned
bool gameOver=false;	 // Flag to track whether the game has finished yet
bool whiteTurn = true; // Set to true if it is white's turn, false if it is black's turn


int selectedPawn = -1; // Index of the selected pawn (-1 if no pawn is selected)
int cursorX = 0;
int cursorY = 0;




void reshape(int width, int height) {
	// Set the viewport to cover the entire window
	glViewport(0, 0, width, height);

	// Set the projection matrix to match the window aspect ratio
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-10,40,-0,40);
	// Return to the modelview matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void movePawn(int x, int y) {



	// Check if it is white's turn
	if (whiteTurn) {
		// Check if the selected pawn is a valid white pawn
		if (selectedPawn < 0 || selectedPawn >= 3) {
			return;
		}

		// Check if the destination square is empty
		bool squareOccupied = false;
		for (int i = 0; i < 3; i++) {
			if (x == whitePawnPositions[i][0] && y == whitePawnPositions[i][1]) {
				squareOccupied = true;
				break;
			}
			if (x == blackPawnPositions[i][0] && y == blackPawnPositions[i][1]) {
				squareOccupied = true;
				break;
			}
		}
		if (squareOccupied) {
			return;
		}

		// Move the pawn to the destination square
		previousWhitePawnPositions[selectedPawn][0] = whitePawnPositions[selectedPawn][0];
		previousWhitePawnPositions[selectedPawn][1] = whitePawnPositions[selectedPawn][1];
		whitePawnPositions[selectedPawn][0] = x;
		whitePawnPositions[selectedPawn][1] = y;

		// Check if all white pawns have reached the end of the board

		for (int i = 0; i < 3; i++) {
			if (whitePawnPositions[i][1] == boardSize-1) {
				whitePawnsAtEnd++;
			}
		}
		if (whitePawnsAtEnd == 1) {
			// printf("Game over! White wins!\n");
			gameOver = true;
			return;
		}

		// Change the turn to black
		whiteTurn = false;
	}
	else {
		// Check if the selected pawn is a valid black pawn
		if (selectedPawn < 3 || selectedPawn >= 6) {
			return;
		}

		// Check if the destination square is empty
		bool squareOccupied = false;
		for (int i = 0; i < 3; i++) {
			if (x == whitePawnPositions[i][0] && y == whitePawnPositions[i][1]) {
				squareOccupied = true;
				break;
			}
			if (x == blackPawnPositions[i][0] && y == blackPawnPositions[i][1]) {
				squareOccupied = true;
				break;
			}
		}
		if (squareOccupied) {
			return;
		}

		// Move the pawn to the destination square
		previousBlackPawnPositions[selectedPawn - 3][0] = blackPawnPositions[selectedPawn - 3][0];
		previousBlackPawnPositions[selectedPawn - 3][1] = blackPawnPositions[selectedPawn - 3][1];
		blackPawnPositions[selectedPawn - 3][0] = x;
		blackPawnPositions[selectedPawn - 3][1] = y;

		// Check if all white pawns have reached the end of the board

		for (int i = 0; i < 3; i++) {
			if (blackPawnPositions[i][1] == 0) {
				blackPawnsAtEnd++;
			}
		}
		if (blackPawnsAtEnd == 1) {
			// printf("Game over! White wins!\n");
			gameOver = true;
			return;
		}

		// Change the turn to white
		whiteTurn = true;
	}
	glutPostRedisplay();
}



void keyboard(unsigned char key, int x, int y) {



	// Only update the position of the selected pawn or the cursor if the game is not over
	switch (key) {
	case 'w':
		if (selectedPawn >= 0 && selectedPawn < 3) {
			movePawn(cursorX  , cursorY+ 1);
		} else {
			// Move the cursor down
			cursorY = (cursorY + 1) % boardSize;
		}
		break;
	case 'a':
		// Move the selected pawn left
		if (selectedPawn >= 0&&whitePawnPositions[selectedPawn][0] != 0||blackPawnPositions[selectedPawn - 3][0] != 0) {
			movePawn((cursorX + boardSize - 1) % boardSize, cursorY);
		} else {
			// Move the cursor up
			cursorX = (cursorX + boardSize - 1) % boardSize;
		}
		break;
	case 's':
		// Move the selected pawn down
		if (selectedPawn >= 3 && selectedPawn < 6) {	
			movePawn(cursorX, cursorY - 1);
		} else {
			// Move the cursor left
			cursorY = (cursorY + boardSize - 1) % boardSize;
		}
		break;
	case 'd':
		// Move the selected pawn right
		if (selectedPawn >= 0&&whitePawnPositions[selectedPawn][0] != boardSize-1&&blackPawnPositions[selectedPawn - 3][0] != boardSize-1) {
			movePawn((cursorX + 1) % boardSize, cursorY);
		} else {
			// Move the cursor right
			cursorX = (cursorX + 1) % boardSize;
		}
		break;

	case 'e':if (selectedPawn >= 0 && selectedPawn < 3) {
		// Check if there is a black pawn at the target position
		bool captured = false;
		for (int i = 0; i < 3; i++) {
			if (blackPawnPositions[i][0] == cursorX + 1 && blackPawnPositions[i][1] == cursorY + 1) {
				// Capture the black pawn
				blackPawnPositions[i][0] = -1;
				blackPawnPositions[i][1] = -1;
				captured = true;
				break;
			}
		}
		if (captured) {
			// Move the white pawn to the target position
			movePawn(cursorX + 1, cursorY + 1);
		}
			 } else if (selectedPawn >= 3 && selectedPawn < 6) {
				 // Check if there is a white pawn at the target position
				 bool captured = false;
				 for (int i = 0; i < 3; i++) {
					 if (whitePawnPositions[i][0] == cursorX + 1 && whitePawnPositions[i][1] == cursorY - 1) {
						 // Capture the white pawn
						 whitePawnPositions[i][0] = -1;
						 whitePawnPositions[i][1] = -1;
						 captured = true;
						 break;
					 }
				 }
				 if (captured) {
					 // Move the black pawn to the target position
					 movePawn(cursorX + 1, cursorY - 1);
				 }
			 }

			 break;
	case 'q':if (selectedPawn >= 0 && selectedPawn < 3) {
		// Check if there is a black pawn at the target position
		bool captured = false;
		for (int i = 0; i < 3; i++) {
			if (blackPawnPositions[i][0] == cursorX - 1 && blackPawnPositions[i][1] == cursorY + 1) {
				// Capture the black pawn
				blackPawnPositions[i][0] = -1;
				blackPawnPositions[i][1] = -1;
				captured = true;
				break;
			}
		}
		if (captured) {
			// Move the white pawn to the target position
			movePawn(cursorX - 1, cursorY + 1);
		}
			 } else if (selectedPawn >= 3 && selectedPawn < 6) {
				 // Check if there is a white pawn at the target position
				 bool captured = false;
				 for (int i = 0; i < 3; i++) {
					 if (whitePawnPositions[i][0] == cursorX - 1 && whitePawnPositions[i][1] == cursorY - 1) {
						 // Capture the white pawn
						 whitePawnPositions[i][0] = -1;
						 whitePawnPositions[i][1] = -1;
						 captured = true;
						 break;
					 }
				 }
				 if (captured) {
					 // Move the black pawn to the target position
					 movePawn(cursorX - 1, cursorY - 1);
				 }
			 }
			 break;
	case 'z':
		if(whiteBack!=0){
		if (selectedPawn >= 0 && selectedPawn < 3) {
			// Move the selected white pawn back to its previous position
			whitePawnPositions[selectedPawn][0] = previousWhitePawnPositions[selectedPawn][0];
			whitePawnPositions[selectedPawn][1] = previousWhitePawnPositions[selectedPawn][1];
			whiteBack--;
		}
		}
		if(blackBack!=0){
		 if (selectedPawn >= 3 && selectedPawn < 6) {
			// Move the selected black pawn back to its previous position
			blackPawnPositions[selectedPawn - 3][0] = previousBlackPawnPositions[selectedPawn - 3][0];
			blackPawnPositions[selectedPawn - 3][1] = previousBlackPawnPositions[selectedPawn - 3][1];
			blackBack--;
		}
		}
		whiteTurn = !whiteTurn;
		break;
	default:
		break;
	case 32: // Space bar
		// Check if the cursor is over a pawn
		for (int i = 0; i < 3; i++) {
			if (cursorX == whitePawnPositions[i][0] && cursorY == whitePawnPositions[i][1]) {
				// Select the white pawn at this position
				selectedPawn = i;
				break;
			}
		}

		for (int i = 0; i < 3; i++) {
			if (cursorX == blackPawnPositions[i][0] && cursorY == blackPawnPositions[i][1]) {
				// Select the black pawn at this position
				selectedPawn = i + 3;
				break;
			}
		}
		break;

	case 27: // Escape key
		// Deselect the pawn
		selectedPawn = -1;
		break;
	case 'r':

		// Reset the game
		pawnsSpawned = false;
		gameOver = false;
		whiteTurn = true;
		selectedPawn = -1;
		whitePawnsRemaining = 3;
		blackPawnsRemaining = 3;
		whitePawnsAtEnd = 0;
		blackPawnsAtEnd = 0;
		whiteBack=3;
		blackBack=3;
		break;

	}

	glutPostRedisplay();
}




void drawBoard()
{
	// Draw the chess board
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			if ((i + j) % 2 == 0) {
				// Draw a white square
				glColor3f(1.0, 1.0, 1.0);
			} else {
				// Draw a black square
				glColor3f(0.0, 0.0, 0.0);
			}


			// Draw the square
			glBegin(GL_POLYGON);
			glVertex2f(i * squareSize, j * squareSize);
			glVertex2f((i + 1) * squareSize, j * squareSize);
			glVertex2f((i + 1) * squareSize, (j + 1) * squareSize);
			glVertex2f(i * squareSize, (j + 1) * squareSize);
			glEnd();
		}
	}

	// Draw the white pawns

	for (int i = 0; i < 3; i++) {
		if (i  == selectedPawn) {
			// Draw the selected pawn in a different color
			glColor3f(1.0, 0.0, 0.0);
		} else {
			glColor3f(0.0,1.0, 0.0);
		}
		glBegin(GL_POLYGON);
		for (int j = 0; j < numVertices; j++) {
			float angle = 2 * M_PI * j / numVertices;
			glVertex2f(whitePawnPositions[i][0] * squareSize + squareSize * 0.5 + squareSize * 0.25 * cos(angle),whitePawnPositions[i][1]* squareSize+2.5  + squareSize * 0.25 * sin(angle));
		}
		glEnd();
	}

	// Draw the black pawns

	for (int i = 0; i < 3; i++) {
		if (i + 3 == selectedPawn) {
			// Draw the selected pawn in a different color
			glColor3f(1.0, 0.0, 0.0);
		} else {
			glColor3f(0.0, 0.0, 1.0);
		}

		glBegin(GL_POLYGON);
		for (int j = 0; j < numVertices; j++) {
			float angle = 2 * M_PI * j / numVertices;
			glVertex2f(blackPawnPositions[i][0] * squareSize + squareSize * 0.5 + squareSize * 0.25 * cos(angle), blackPawnPositions[i][1] * squareSize+2.5  + squareSize * 0.25 * sin(angle));
		}
		glEnd();
	}
	if (!pawnsSpawned) {
		srand(time(NULL)); // Seed the random number generator
		for (int i = 0; i < 3; i++) {
			whitePawnPositions[i][0] = rand() % 8; // Generate a random position for the white pawn
			whitePawnPositions[i][1] = 0; // Set the y-position to the first line

			blackPawnPositions[i][0] = rand() % 8; // Generate a random position for the black pawn
			blackPawnPositions[i][1] = 7; // Set the y-position to the

			// Make sure the positions are unique
			for (int j = 0; j < i; j++) {
				if (whitePawnPositions[i][0] == whitePawnPositions[j][0]) {
					whitePawnPositions[i][0] = (whitePawnPositions[i][0] + 1) % 8; // If the position is already taken, try the next one
				}
				if (blackPawnPositions[i][0] == blackPawnPositions[j][0]) {
					blackPawnPositions[i][0] = (blackPawnPositions[i][0] + 1) % 8; // If the position is already taken, try the next one
				}
			}
		}
		pawnsSpawned = true; // Set the flag to indicate that the pawns have been spawned
	}


}		

void display() {



	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	if (gameOver) {
		// Set the raster position to the left corner of the screen
		glRasterPos2i(-10, 39);

		// Set the font size and the text color
		// glScalef(2.0, 2.0, 2.0);
		glColor3f(1.0, 1.0, 1.0); // White color

		// Set the message based on the conditions
		char* message;
		if (whitePawnsAtEnd==1) {
			message = "Game over! Green wins!";
		}else if (blackPawnsAtEnd==1) {
			message = "Game over! Blue wins!";
		}

		// Draw the message one character at a time
		for (int i = 0; i < strlen(message); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
		}
	}

	if (whiteTurn) {
		// Set the text color to white
		glColor3f(1.0, 1.0, 1.0);
		// Set the position of the text
		glRasterPos2i(-10, 38);
		// Draw the text
		for (int i = 0; i < strlen("Green's turn"); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "Green's turn"[i]);
		}
	}
	else {
		// Set the text color to black
		glColor3f(1.0, 1.0, 1.0);
		// Set the position of the text
		glRasterPos2i(-10, 38);
		// Draw the text
		for (int i = 0; i < strlen("Blue's turn"); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "Blue's turn"[i]);
		}
	}
	glRasterPos2i(-10, 35);
	char buffer[100];
	sprintf(buffer, "Green's Backs: %d", whiteBack);
	for (int i = 0; i < strlen(buffer); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);
	}
	glRasterPos2i(-10, 34);
	char buffer1[100];
	sprintf(buffer1, "Blue's Backs: %d", blackBack);
	for (int i = 0; i < strlen(buffer1); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer1[i]);
	}
	glRasterPos2i(-10, 16);
	// Draw the text
	for (int i = 0; i < strlen("Press any key to start"); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "Press any key to start"[i]);
	}
	// Set the position of the text
	glRasterPos2i(-10, 15);
	// Draw the text
	for (int i = 0; i < strlen("W A S D for moving the pawn"); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "W A S D for moving the pawn"[i]);
	}
	glRasterPos2i(-10, 14);
	// Draw the text
	for (int i = 0; i < strlen("Q for killing the left diagonal pawn"); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "Q for killing the left diagonal pawn"[i]);
	}
	glRasterPos2i(-10, 13);
	// Draw the text
	for (int i = 0; i < strlen("E for killing the right diagonal pawn"); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "E for killing the right diagonal pawn"[i]);
	}
	glRasterPos2i(-10, 12);
	// Draw the text
	for (int i = 0; i < strlen("Space for selecting the pawn"); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "space for selecting the pawn"[i]);
	}
	glRasterPos2i(-10, 11);
	// Draw the text
	for (int i = 0; i < strlen("Z to back the selected pawn ine step "); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "Z to back the selected pawn ine step"[i]);
	}
	glRasterPos2i(-10, 10);
	// Draw the text
	for (int i = 0; i < strlen("ESC to diselect the pawn"); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "ESC to diselect the pawn"[i]);
	}
	glRasterPos2i(-10, 9);
	// Draw the text
	for (int i = 0; i < strlen("R + anykey to reset the game"); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "R + anykey to reset the game"[i]);
	}
	glRasterPos2i(-10, 5);
	// Draw the text
	for (int i = 0; i < strlen("Note:if u select the pawn and didnt move"); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "Note:if u select the pawn and didnt move"[i]);
	}

	glRasterPos2i(-10, 4);
	for (int i = 0; i < strlen("               better to diselect it"); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "           better to diselect it"[i]);
	}






	drawBoard();
	// Draw the cursor
	if (selectedPawn >= 0) {
		// Draw a circle around the selected pawn

	} else {
		// Draw the cursor
		glColor4f(1.0, 1.0, 0.0, 0.5);
		glBegin(GL_POLYGON);
		glVertex2f(cursorX * squareSize, cursorY * squareSize);
		glVertex2f((cursorX + 1) * squareSize, cursorY * squareSize);
		glVertex2f((cursorX + 1) * squareSize, (cursorY + 1) * squareSize);
		glVertex2f(cursorX * squareSize, (cursorY + 1) * squareSize);
		glEnd();
	}


	glDisable(GL_BLEND);

	glutSwapBuffers();

}

int main(int argc, char** argv) {
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Set the window size and position						
	glutInitWindowSize(640, 640);
	// glutInitWindowPosition(100, 100);


	// Create the window
	glutCreateWindow("Chess Board");

	//glOrtho(-10.0, 10, -10.0, 10.0, -.0, 1.0);

	// Set the display function
	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	// Set the mouse click callback function
	//glutMouseFunc(mouse);
	// Set the key press callback function
	//glutKeyboardFunc(keyPress);
	// Run the main loop
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
