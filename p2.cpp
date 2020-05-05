// Pentru o fereastra de dimensiuni 24x24 vom avea aprox. 160 000 de caracteristici (la scala minima)
// Vom cauta toate caracteristicile pentru toate dimensiunile
// Pentru fiecare fereastra de 24x24 vom avea aprox. 2500 clasificatori slabi

#include<opencv2/core/mat.hpp>
#include<algorithm>
#include<fstream>
#include<cmath>

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

/*class pixel{
	int r;
	int g;
	int b;
public:
	pixel(int r, int g, int b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
	int getR() {
		return r;
	}
	int getG() {
		return g;
	}
	int getB() {
		return b;
	}
};


//Formarea Caracteristicilor
class Feature {
    int type; // am declarat tipurile de caracteristici
    int width, height; // latimea si inaltimea fiecarei caracteristici
    int xc, yc; // coordonatele x si y ale coltului din stanga-sus ale caracteristicii in subfereastra de detectie
    float rectangleValue(float* ii, int iiwidth, int ix, int iy, int rx, int ry, int rw, int rh);// ii=imaginea integrala
public:
    Feature(int type, int xc, int yc, int width, int height);// constructor
    float getValue(float* ii, int iiwidth, int x, int y);
    int getType();
    int getWidth();
    int getHeight();
    void scale(float s);
    std::string toString();
};

Feature::Feature(int type, int xc, int yc, int width, int height) { // definim constructorul
    this->type = type;
    this->xc = xc;
    this->yc = yc;
    this->width = width;
    this->height = height;
}

string int_to_string(int i) {// functie de conversie din int in string
    stringstream ss;
    ss << i;
    string str = ss.str();
    return str;
}

string Feature::toString() {// concantenam toate valorile string ale parametrilor unei caracteristici
    string str;
    str = int_to_string(type) + " " + int_to_string(width) + " " + int_to_string(height) + " " + int_to_string(xc) + " " + int_to_string(yc);
    return str;
}

int Feature::getType() { // returnam tipul (pentru ca este declarat privat in clasa)
    return this->type;
}

int Feature::getWidth() { // returnam latimea (pentru ca este declarata privat in clasa)
    return this->width;
}

int Feature::getHeight() { // returnam inaltimea (pentru ca este declarata privat in clasa)
    return this->height;
}

/*float Feature::rectangleValue(float *ii, int iiwidth, int ix, int iy, int rx, int ry, int rw, int rh) {
  return;
}*/

/*float Feature::getValue(float *ii, int iiwidth, int x, int y) {
  if(this->type == 1)
      return rectangleValue()-rectangleValue();
  else if(this->type == 2)
        return rectangleValue()-rectangleValue();
  else if(this->type == 3)
      return rectangleValue()-rectangleValue()-rectangleValue();
  else if(this->type == 4)
      return;
      .
      .
      .
   else {
      cout << "Eroare: caracteristica de tip " << this->type << " nu exista. Tipul caracteristicilor se afla in intervalul [,].\n";
      exit(-1);
  }
}

void Feature::scale(float s) {
    this->width *= s;
    this->height *= s;
    this->xc *= s;
    this->yc *= s;
}
 
//Clasificatorii slabi      -> in acest caz pentru a forma clasificatorii slabi vom avea nevoie de:
//    -> pondere  (initializarea) fiecare clasifictor are ponderi iar acestea le vom pune egale (uniforme) ca apoi clasificatorul puternic sa tinda catre 0
//    ->exita o eroare cuprinsa intre [0,1] pentru fiecare clasificator slab in parte care ajuta la antrenarea lui
//    -> rata de eroare 

class WeakClassifier {

protected: float error_rate;
         float weight;  
         
public:
    //constructor
    
    WeakClassifier(float err, float wgh)
    {
       WeakClassifier::error_rate = err;
       WeakClassifier::weight = wgh;
   };
    float getErr() const;
    float getWeight() const;
    void train_Classifier() const;
    

    void setErr(long err);
    void setWeight(float wgh);
 
    void printClassifier();
    void writeClassifier();

//destructor
~WeakClassifier() {}
};


void WeakClassifier::setErr(long err) {
   this->error_rate= err;
}

void WeakClassifier::setWeight(float wgh) {
    this->weight = wgh;
}
float WeakClassifier::getErr() const {
    return error_rate;                 
}

float WeakClassifier::getWeight() const {

    return weight;
}

//Antrenarea clasificatorilor slabi->dorim sa oferim anumite valori prin diferite ecuatii matematice, aceste valori depinzand de eroarea caracteristicii
void WeakClassifier:: train_Classifier() const  {
    WeakClassifier Weak1();
    Weak1.setWeight();
    //weight =1/2*log((1-error_rate)/error_rate);   
}
//weight =1/2*log((1-error_rate)/error_rate);   pentru antrenare */



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

	/*for (int j = 0; j < height; j++) {
		vector<pixel> vec;
		for (int i = 0; i < width*channelsNo; i += channelsNo) {
			int r, g, b;
			r = read_image.at<char>(j, i + 2)/255 * 0.2126;
			g = read_image.at<char>(j, i + 1)/255 * 0.7152;
			b = read_image.at<char>(j, i + 0)/255 * 0.0722;
			pixel img_pixel(r, g, b);
			vec.push_back(img_pixel);
		}
		image.push_back(vec);
		vec.clear();
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width*channelsNo; i += channelsNo) {
			cout << image.at(j).at(i).getR() << " " << image.at(j).at(i).getG() << " " << image.at(j).at(i).getB() << "; ";
		}
		cout << "\n";
	}*/


	/*for (int j = 0; j < height; j++) {
		for (int i = 0; i < width*channelsNo; i += channelsNo) {
			//cout << (int)read_image.at<char>(j, i + 2) << " " << (int)read_image.at<char>(j, i + 1) << " " << (int)read_image.at<char>(j, i + 0) << "; ";
			unsigned char red = read_image.imageData[j*read_image.widthStep + i + 2];

			read_image.at<char>(j, i + 1) *= 0.2152;
			read_image.at<char>(j, i + 0)  *= 0.2722;
		}
		//cout << "\n";
	}*/




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

	/*Vec3b intensity = image.at<Vec3b>(y, x);

	float blue = intensity.val[0];
	float green = intensity.val[1];
	float red = intensity.val[2];

	std::cout << "Blue:" << blue << std::endl;
	std::cout << "Green:" << green << std::endl;
	std::cout << "Red:" << red << std::endl;*/
	//cin.get();


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
