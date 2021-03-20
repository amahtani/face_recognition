/**
 * \file      face.cpp
 * \author    Ankur Mahtani
 * \date      25 Décembre 2017 
 *
 * \details    Recentrage de l'image en fonction de la position du visage de l'utilisateur
  */

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))  
#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : -1)
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))  
#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : -1)

#define abs(x) ((x) > 0 ? (x) : -(x))

using namespace cv;
using namespace std;


int main() {



     FILE *commande = NULL;
     commande = fopen("/dev/video0","w"); //ouverture du port de la webcam
     if(commande == NULL) {
        printf("erreur chargement fichier\n"); //en cas d'erreur
      return 0;
     }



    vector<Mat> images; /// déclaration des images et des labels
    vector<int> labels;
    
    
    CascadeClassifier head_cascade; /// déclarations des xml, sous programmes pour la reconnaissance de visage et d'yeux
    CascadeClassifier eye_cascade;
    head_cascade.load("/home/mahtani/Bureau/OpenCV/opencv-3.3.1/data/haarcascades/haarcascade_frontalface_default.xml"); // charger le file xml d'open cv
    eye_cascade.load("/home/mahtani/Bureau/OpenCV/opencv-3.3.1/data/haarcascades/haarcascade_eye.xml");
    VideoCapture cap(0); //ouverture de la caméra
   
	printf("plot\n");
    if(!cap.isOpened()) {  //vérification de l'ouverture de la caméra
        printf("La caméra n'a pas pu s'ouvrir");
        return -1;
    }

    Mat frame; //création de l'image traitée
   


    while(true) {
        cap >> frame;
        Mat original = frame.clone(); //création d'une image originale
        
        //cv::Mat frame, eye_tpl;
        //cv::Rect eye_bb;
        Point centre = Point(318, 218);  //création d'un point au centre de l'image
        circle(original, centre, 60,CV_RGB(0,255,0),2,8,0); //création d'un cercle d'incertitude de rayon 50 au centre de cette même image 


        Mat gray;  //création de l'image en niveau de gris
        cvtColor(original, gray, CV_BGR2GRAY);  //détection du visage avec des niveaux de gris
        vector< Rect_<int> > faces; //tableau des visages et des yeux
        vector< Rect_<int> > eyes;
        int maximum_area = 0;
        int max_eye1=0;
        int max_eye2=0;
        
        int face_rec;
        head_cascade.detectMultiScale(gray, faces);  //ici on ne sélectionnera qu'une seule personne, sa tête, considérée comme la plus grande trouvée pour le programme
        if (faces.size()!=0){
            for(unsigned int i = 0; i < faces.size(); i++) {

                if (maximum_area< faces[i].width*faces[i].height ){
                    maximum_area = faces[i].width*faces[i].height;
                    face_rec = i;
                }
            }
            
            Rect face_i = faces[face_rec];
            Mat face = (face_i);

            rectangle(original, face_i, CV_RGB(0, 255,0), 1);
            eye_cascade.detectMultiScale(face, eyes);
           // printf("%lu\n",eyes.size());
            
            if (eyes.size() !=0){   // détection des yeux
                for(unsigned int j=0; j<eyes.size();j++){
                    Rect eye_j = eyes[j];
                    Mat eye = gray(eye_j);
                    rectangle(face,eye_j,CV_RGB(255,0,0),1);
                }
            }
                        imshow("face",face);
            int pos_x = face_i.tl().x+(face_i.width/2) ;
            int pos_y = face_i.tl().y+(face_i.width/2) ;
            Point center = Point(pos_x, pos_y);
			Point text = Point (pos_x -(face_i.width/2),pos_y+(face_i.width/2));


            if ( (abs(pos_x-318)<40) && (abs(pos_y-218)) <40) {
                circle(original, center, 10,CV_RGB(0,255,0),-1,8,0); //cercle au centre de l'image
            }
            else circle(original, center, 10,CV_RGB(255,0,0),-1,8,0); //meme cercle
            
            char* txt = "UNKNOWN";
            
            putText(original, txt, text, FONT_HERSHEY_PLAIN, 3.0, CV_RGB(255,0,0), 2.0); //affichage du statut de l'utilisateur

        }


         /*
        *\Affichage de l'image finale
         */ 
        
       // cv::rectangle(original, eye_bb, CV_RGB(0,255,0));
        
        imshow("Reconnaissance Faciale", original);
        //imshow("gray",gray);
        

        char key = (char) waitKey(20);
        
        if(key == 27)
            break;
    }
    
    
return 0;

}

/*


      for (size_t i = 0; i < faces.size(); i++)
      {
           Mat faceROI = frame_gray(faces[i]);
           std::vector<Rect> eyes;
           //-- Dans chaque visage, detecter les yeux
           eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
           if (eyes.size() == 2)
           {
                //-- Dessiner le visage
                Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
                ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 0), 2, 8, 0);
                for (size_t j = 0; j < eyes.size(); j++)
                { //-- Dessiner les yeux
                     Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
                     int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
                     circle(frame, eye_center, radius, Scalar(255, 0, 255), 3, 8, 0);
                }
           } 
      }
      //-- Afficher la cam
     imshow(window_name, frame);
}*/