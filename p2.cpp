// Pentru o fereastra de dimensiuni 24x24 vom avea aprox. 160 000 de caracteristici (la scala minima)
// Vom cauta toate caracteristicile pentru toate dimensiunile
// Pentru fiecare fereastra de 24x24 vom avea aprox. 2500 clasificatori slabi

#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>
#include <cstdlib>

using namespace std;
using namespace cv;

// aici se construieste imaginea integrala, in functie de imaginea initiala si dimensiunile dorite 
// unde ii == imaginea integrala, careia ii adaugam un & pentru a-si pastra valoarea si la iesirea din functie
void to_ii(vector<vector<int>> &img, vector<vector<int>> &ii, int height, int width) {
	int value;
	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			value = img[x][y];
			if (y > 0)
				value += ii[x][y - 1];
			if (x > 0)
				value += ii[x - 1][y];
			if (x > 0 && y > 0)
				value -= ii[x - 1][y - 1];
			ii[x][y] = value;
		}
	}
}

// se calculeaza valoarea dreptunghiului aflat intre punctele A, B, C, D, cu ajutorul imaginii integrale
// pentru a ne fi mai usor sa calculcam valorile caracteristicilor mai tarziu
int rectangle_value(vector<vector<int>> ii, int rectangle_X, int rectangle_Y, int rectangle_Width, int rectangle_Height) {
	int A = 0, B = 0, C = 0, D = 0;
	if (rectangle_X > 0 && rectangle_Y> 0)
		A = ii[rectangle_X - 1][rectangle_Y - 1]; cout << A << " ";
	if (rectangle_Y > 0)
		B = ii[rectangle_X - 1][rectangle_Y + rectangle_Height - 2]; cout << B << " ";
	if (rectangle_X > 0)
		C = ii[rectangle_X + rectangle_Width - 2][rectangle_Y - 1]; cout << C << " ";

	D = ii[rectangle_X + rectangle_Width - 2][rectangle_Y + rectangle_Height - 2]; cout << D <<" \n";

	int sum = A - B - C + D;

	return sum;
}

// aici vom construi clasa ViolaJones, care va contine, printre altele, functia de antrenare a clasificatorilor
class ViolaJones {
public:
	int T = 10; // numarul de runde pentru care se va efectua antrenarea, unde se vor retine "greutatea" si polaritatea fiecarui clasificator slab
	void train(vector<vector<int>> ii, vector<int> training) {
		vector<vector<vector<int>>, int> training_data; // vector de perechi: imagine & clasificarea ei (1 pentru img. poz, 0 pt. neg.)


	}

};



int main() {
	cout << "Se citeste imaginea.";
	cin.get();

	// vector<vector<pixel>> image; // declar matricea de pixeli

	// se citeste imaginea
	Mat read_image; 
	read_image = imread("Data\\pixels_4.png", IMREAD_COLOR);

	// se verifica daca input-ul este valid
	if (!read_image.data) {								// se verifica daca input-ul este valid
		cout << "Imaginea nu a putut fi incarcata." << endl;
		exit(0);
	}
	
	// se obtin latimea, inaltimea imaginii si numarul de canale
	int width = read_image.cols, height = read_image.rows, channelsNo = read_image.channels();
	cout << "Dimensiunile imaginii sunt:\nlatime: " << width << "\ninaltime: " << height << "\nchannels no: " << channelsNo << "\n";

	// are loc conversia BGR -> greyscale
	Mat grey_image;
	cvtColor(read_image, grey_image, COLOR_BGR2GRAY); 

	// am incercat sa facem conversia si fara OpenCV, dar am sters liniile de cod folosite pentru asta
	// si vom reincerca in cazul in care este necesar
	// incercarea de conversie se poate vedea la commit-urile modificate


	vector<vector<int>> mat; // declar matricea de pixeli
	for (int i = 0; i < height; i++) // creez matricea cu valorile intregi corespunzatoare pixelilor greyscale ai imaginii
	{
		vector<int> line;
		for (int j = 0; j < width; j++)
		{
			line.push_back((int)grey_image.at<uchar>(i, j));
		}
		mat.push_back(line);
		line.clear();
	}

	/*for (int i = 0; i < height; i++) { // afisez valori matrice pixeli greyscale
		for (int j = 0; j < width; j++) {
			cout << mat[i][j] << " ";
		}
		cout << "\n";
	}*/

	//cout << mat[0][0] << " " << mat[0][839] << " \n" << mat[413][0] << " " << mat[413][839]; // test

	namedWindow("Afisare imagine", WINDOW_AUTOSIZE); // se creaza o fereastra pentru afisare
	imshow("Afisare imagine", grey_image);				 // se afiseaza imaginea citita in fereastra de afisare
	waitKey(0);											// se asteapta apasarea unei taste
	//read_image.release();
	//grey_image.release();


	vector<vector<int>> integral_image(mat);

	// initial ii == mat
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			cout << integral_image[i][j] << " ";
		}
		cout << "\n";
	}
	cin.get();

	// obtinem imaginea integrala
	to_ii(mat, integral_image, height, width);

	// se afiseaza imaginea integrala
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			cout << integral_image[i][j] << " ";
		}
		cout << "\n";
	}
	cin.get();

	// aici trb sa se intample cv. cu suma diferitelor dreptunghiuri, calc. cu ajutorul ii
/*	// test pentru fereastra de 3x3 care incepe de la coordonatele (1,1) == coltul stanga sus
	int sum = rectangle_value(integral_image, 1, 1, 3, 3);
	cout << "sum = " << sum; 
	cin.get(); */

	// aici incepe partea din main() pentru antrenarea clasificatorilor, unde vom incepe cu "the sliding window"
	int min_sliding_window_size = , max_sliding_window_size = ;



	return 0;
}
