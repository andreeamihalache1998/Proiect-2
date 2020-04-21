// Pentru o fereastra de dimensiuni 24x24 vom avea aprox. 160 000 de caracteristici (la scala minima)
// Vom cauta toate caracteristicile pentru toate dimensiunile
// Pentru fiecare fereastra de 24x24 vom avea aprox. 2500 clasificatori slabi

#include<opencv2/opencv.hpp>
#include<opencv2/core/mat.hpp>
#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<algorithm>
#include<fstream>
#include<cmath>
#include <cstdlib>

using namespace std;
using namespace cv;
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

int main()
{

	//afisarea imaginii
	Mat img = imread("E:\\FACULTATE\\lena.png", 1);
	if (!img.data)
	{
		cout << "No image data \n";
		return -1;
	}
	//Convertim imaginea in alb-negru- GreyScale
	Mat img_gray;
	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	namedWindow("Display Original Image", WINDOW_AUTOSIZE);
	imshow("Display Original Image", img);

	//Display the grayscale image 
	namedWindow("Display Grayscale Image", WINDOW_AUTOSIZE);
	imshow("Display Grayscale Image", img_gray);
    waitKey(0);
	return 0;


}
