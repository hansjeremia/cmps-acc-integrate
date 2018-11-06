// keterangan :
// permasalahan saat ini, kalo robot diem terus gerak terus diem lagi akan ada waktu dimana
// robot sudah diam tapi bacaannya belum berhenti jadi jaraknya akan terus bertambah
// adanya delay 0,5 sekon yang cukup berpengaruh terhadap pembacaan
// harus dicoba dirobot untuk mengetahui galat dari cara ini
// mounting pada robot harus diperhatikan karena bila gerak sedikit maka akan mempengaruhi bacaan
// ada kemungkinan bacaan sumbu x menjadi sumbu y jadi harus di buat kodingannya !
// waktu belok harus pake g sin teta / semacam itu soalnya kodingan ini masih buat yg lurus
// ay = ayc cos teta - axcsin teta
// ax = axccos teta + aycsin teta
// coba pakai kompas
// cobain ngereset pas dilapangan buat datanya supaya dikoreksi (e.g, ada rintanan di jarak 5m terus pas nyampe situ datanya direset)
// ********************************************************************************
// *                                                                              *
// *                                                                              *
// *                                                                              *
// *                                                                              *
// *                                                                              *
// *                                                                              *
// *                                                                              *
// *                                                                              *
// *                                                                              *
// *                                                                              *
// *                                                                              *
// *                           by. Divisi Elektrik Robot Kuda - KRAI ITB 2018     *
// ********************************************************************************

// library
#include "CMPS_KRAI.h"
#include "mbed.h"
#include "millis.h"
#include "rtos.h"

// pin yang digunakan
#define pin_sda PC_9 //PIN SDA
#define pin_scl PA_8 //PIN SCL

CMPS_KRAI cmps (pin_sda, pin_scl, 0xc0);
Serial pc (USBTX,USBRX,115200);

char data[24];
int main(){
float ax, ay, az = 0; //percepatan dengan sumbunya
float vx, vy, vz = 0; //kecepatan dengan sumbunya
float X, Y, Z = 0;    // koordinat robot saat ini
int a = 0;
int millisx = 0;
int TS = 1000;
float v1, v2, v3;
float s1, s2, s3;

while (1){   
    while (a==0){
        cmps.getData(data);
        float a1 = ((int)data[12] << 8 | (int)data[11]);
        float b1 = ((int)data[14] << 8 | (int)data[13]);
        Thread::wait(1000); // delay 1s
        if (((b1 > 1000) || (b1 < 700))&&((a1 < 50000)||(a1>80000))){ 
        a = 1;
    }
        
    startMillis();
    while(a==1){
            // baca data percepatan melalui compass
            cmps.getData(data);
                float a1 = ((int)data[12] << 8 | (int)data[11]);
                float b1 = ((int)data[14] << 8 | (int)data[13]);
                float c1 = ((int)data[16] << 8 | (int)data[15]);
                Thread::wait(1000); // delay 1s 
                
            //conditional, apabila bacaan sumbu y diatas 10 m/s^2 atau bacaan dibawah 7 m/s^2
            //              maka menandakan robot sedang mulai berjalan dan mulai perhitungan koordinat
            if ((b1 > 1000) || (b1 < 700)){
                 v1 = ((a1+ax)/4);         //metode integral trapesium dengan t = 0,5 s
                 ax = a1;
                 v2 = ((b1+ay)/4);
                 ay = b1;
                 v3 = ((c1+az)/4);
                 az = c1;
                 s1 = ((v1+vx)/4);
                 vx = v1;
                 X = X + s1;
                 s2 = ((v2+vy)/4);
                 Y = Y + s2;
                 vy = v2;
                 s3 = ((v3+vz)/4);
                 Z = Z + s3;
                 vz = v3;
               }
            //    pc.printf("s1x=%.2f v1=%.2f a1=%.2f a2=%.2f\n", s1/100, v1/100, ax/100, a1/100);
            //                pc.printf("s1y=%.2f v1=%.2f a1=%.2f a2=%.2f\n", s2/100, v2/100, ay/100, b1/100);
            //                            pc.printf("s1z=%.2f v1=%.2f a1=%.2f a2=%.2f\n", s3/100, v3/100, az/100, c1/100); }
            //apabila bacaan sumbu y di sekitar 9,8 (gravitasi)
            else {                                                          
                pc.printf ("robot masih diem pak, bacaan masih 0");
                pc.printf ("koordinat robot saat ini :");
                }
        if (millis()- millisx > TS){
                                millisx = millis();
                                pc.printf("X=%.2f Y=%.2f Z=%.2f\n", X/100, Y/100, Z/100); }
            }
        }
    }
}