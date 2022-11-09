#include <16f877A.H>
#device adc=10
#FUSES HS,NOLVP,NOPROTECT,NOWDT
#use  delay(clock=4000000)
#define LCD_DB4  PIN_D4
#define LCD_DB5  PIN_D5
#define LCD_DB6  PIN_D6
#define LCD_DB7  PIN_D7
#define LCD_RW  PIN_D21
#define LCD_RS   PIN_D20
#define LCD_E    PIN_D19
#define enter  PIN_B2
#define back  PIN_B3
#BYTE PORTB = 0X06
INT16 valor_adc;
float intensidad;
float voltaje;
int8 temperatu;
void menu();
int boton;
#include <lcd.c>
////////////////Funcion para configuaracion de entradas y salidad////////////////////
 void configuracion ()
 {
 SET_TRIS_B(0XFF); //puerto B como entrada 
 SET_TRIS_A(0XFF); //puerto A como entrada 
 SET_TRIS_D(0X00);// puerto d como salidas 
 SETUP_ADC(ADC_CLOCK_INTERNAL);
 SETUP_ADC_PORTS(ALL_ANALOG);
 lcd_init();  
 }
 ////////////////////////////////funcion para caculo de temperatura //////////////////////////////
float temperatura (boton1)
   {
    if(boton1 == 1)
         {
                setup_adc(adc_clock_internal);
                setup_adc_ports(all_analog);
                int16 a;
                int8 temp;
                lcd_init();
                set_adc_channel(0); //entrada atraves del pin a0
                delay_us(20); 
                a=read_adc(); 
                temp=(a*500)/1023; // caculo de temperatura 
                return temp;        // retornar el valor de la temperatura
               }
    }   
    
/////////////////////////////////////////////Funcion para calulo del voltaje /////////////////////////////////////////
float VOL (boton1)
{
    if(boton1 == 2){                   //verificamos que nuestro contador este en 2 
                  SETUP_ADC(ADC_CLOCK_INTERNAL);
                  SETUP_ADC_PORTS(ALL_ANALOG);
                  lcd_init();
                  SET_ADC_CHANNEL(1);
                  float vol;      //variable auxiliar 
                  delay_us(20);
                  valor_adc=read_adc();
                  vol=20.0*valor_adc/1023.0; //calculo del voltaje 
                  return vol; //retornamos resultado 
                    }
    }
 /////////////////////////////////////////Funcion para el calculo de la corriente //////////////////////////////////
 float Corriente (boton1)
 { 
   if(boton1==3){
          setup_adc(adc_clock_internal);
            setup_adc_ports(all_analog);
            lcd_init();
               while(true){
               set_adc_channel(1);
            //   float vol;
               delay_us(20);
               valor_adc=READ_ADC();
               valor_adc=valor_adc-10;
               //voltaje=80.0*valor_adc/1023.0;
               intensidad=(20.0*valor_adc/1023.0)/9000;
               return intensidad;
     }
  }
}
 /////////////////// FUNCION PARA  EL USO DE LOS BOTONES Y DESPRENDER MENSAJES EN E LCD ////////////////////////////
 void menu()
      {
      boton=1;         //inicializamos nuestro  boton en uno 
      configuracion(); //llamamos a la funcion configuración
     DO{
            //UP= RB7
     IF(BIT_TEST(PORTB,0)==1)  //boton  up precionado 
      {
         boton--;             //disminuimos nuestro auxiliarara poder recorrer el menu  hacia arriba
         delay_ms(40);
      }
            //DOWN = B6 
       IF(BIT_TEST(PORTB,1)==1) // boton dawn precionado 
       {
         boton++;        //aumentamos nuestra auxiliar para poder recorrer el menu  hacia abajo
          delay_ms(40);
       }
        if(boton>3) //si nuestro auxiliar es mayor es 3 lo ponemos en uno nuevamente  
      {
       boton=1;
      } 
      if(boton==1) 
        { 
        printf(lcd_putc,"\f");
         lcd_gotoxy(1,1); //Linea 1, columna 1
        lcd_putc("Temperatura<- ");
         lcd_gotoxy(1,2);
        lcd_putc("Voltaje");
         delay_us(200);
         if(!input(enter))  // cachamos si el boton enter esta siendo precionado 
            {
             while (input(back))   //utiizamos el ciclo whie  para cachar si back a sido precionado si va no esta precionado de lo contrario veremos nuevamente el menu 
               {
                   temperatu=temperatura(boton);// llamamos  a la funcion temperatura 
                   lcd_gotoxy(1,1);   // nos pocicionamos en la pocicion 1,1 del LCD 
                   printf(lcd_putc,"La temperatura \nes: %u C",temperatu); //Imprimimos en el LCD  "la tempratura es : "+ el valor de la temperatura 
                   delay_ms(300);
               }
            
             }
                 
               } 
 
     if(boton==2)
        { 
               printf(lcd_putc,"\f");
               lcd_gotoxy(1,1); //Linea 1, columna 1
               lcd_putc("Voltaje<-");
               lcd_gotoxy(1,2);
               lcd_putc("Corriente ");
               delay_us(200);
               if(!input(enter)) // cachamos si el boton enter esta siendo precionado 
               {
                     while (input(back)) //utiizamos el ciclo whie  para cachar si back a sido precionado si va no esta precionado de lo contrario veremos nuevamente el menu 
                        {
                       voltaje=VOL(boton); //llamamos a la funcion  voltaje 
                       lcd_gotoxy(1,1);
                       printf(lcd_putc,"Valor: %fV",voltaje);
                       delay_ms(50);
                        }
                      boton=1;
               }
         
        }
      if(boton==3)
        {
         printf(lcd_putc,"\f");
         lcd_gotoxy(1,1); //Linea 1, columna 1
         lcd_putc("Voltaje");
         lcd_gotoxy(1,2);
         lcd_putc("Corriente<- ");
         delay_us(200);
         if(!input(enter))
         {
            while(input(back))
               {
                  intensidad=Corriente(boton);
                  lcd_gotoxy(1,1);
                  printf(lcd_putc,"Valor: %0.4fi",intensidad);
                  delay_ms(50);
               }
               //boton=1;
         }
       }
         
   }while(true);
}
 
///////////////////////Funcion principal/////////////////////////////
Void main()
{
 menu();
}
