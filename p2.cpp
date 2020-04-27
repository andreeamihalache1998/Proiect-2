// Pentru o fereastra de dimensiuni 24x24 vom avea aprox. 160 000 de caracteristici (la scala minima)
// Vom cauta toate caracteristicile pentru toate dimensiunile
// Pentru fiecare fereastra de 24x24 vom avea aprox. 2500 clasificatori slabi

#include<opencv2/core/mat.hpp>
#include<algorithm>
#include<fstream>
#include<cmath>
#include <cstdlib>


#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>



using namespace std;
using namespace cv;

class pixel{
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
}*/

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
//weight =1/2*log((1-error_rate)/error_rate);   pentru antrenare



int main() {
	cout << "Se citeste imaginea.";
	//cin.get();

	vector<vector<pixel>> image; // declar matricea de pixeli


	Mat grey_image;

	Mat read_image; 
	read_image = imread("Data\\test.png", IMREAD_COLOR);		// se citeste imaginea

	if (!read_image.data) {								// se verifica daca input-ul este valid
		cout << "Imaginea nu a putut fi incarcata." << endl;
		exit(0);
	}

	int width = read_image.cols, height = read_image.rows, channelsNo = read_image.channels();

	cout << "Dimensiunile imaginii sunt:\nlatime: " << width << "\ninaltime: " << height << "\nchannels no: " << channelsNo << "\n";


	cvtColor(read_image, grey_image, COLOR_BGR2GRAY); // convertire BGR -> greyscale

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




	vector<vector<int>> vec_dragut;
	for (int i = 0; i< height; i++)
	{
		vector<int> linie;
		for (int j = 0; j< width; j++)
		{
			linie.push_back((int)grey_image.at<uchar>(i, j));
		}
		vec_dragut.push_back(linie);
		linie.clear();
	}


	/*for (int i = 0; i < height; i++) { // test afisare valori matrice pixeli greyscale
		for (int j = 0; j < width; j++) {
			cout << vec_dragut[i][j] << " ";
		}
		cout << "\n";
	}*/

	//cout << vec_dragut[0][0] << " " << vec_dragut[0][width-1] << " \n" << vec_dragut[height-1][0] << " " << vec_dragut[height-1][width-1]; //test





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
	return 0;
}
