/*
Dit is de define file waar alle defines voor de main code instaan.
*/

#define  Volg_Modus            -1
#define  Idle                  0
#define  Rijden                1
#define  Actie_Proces_Gewas    2
#define  Actie_Proces_Obstakel 3
#define  Actie_Proces_Koers    4

//Pinnen voor linker stepper  //Mogen digitale pinnen zijn
#define  Pin_Links_Ain2  32
#define  Pin_Links_Ain1  33
#define  Pin_Links_Bin1  34
#define  Pin_Links_Bin2  35

//Pinnen voor retcher stepper
#define  Pin_Rechts_Ain2 40 
#define  Pin_Rechts_Ain1 41
#define  Pin_Rechts_Bin1 44
#define  Pin_Rechts_Bin2 45

#define  Ultrasoon_Voor_Trigger          12
#define  Ultrasoon_Links_Voor_Trigger    10
#define  Ultrasoon_Rechts_Voor_Trigger   8
#define  Ultrasoon_Links_Achter_Trigger  6
#define  Ultrasoon_Rechts_Achter_Trigger 4

#define  Ultrasoon_Voor_Echo             11
#define  Ultrasoon_Links_Voor_Echo       9
#define  Ultrasoon_Rechts_Voor_Echo      7
#define  Ultrasoon_Links_Achter_Echo     5
#define  Ultrasoon_Rechts_Achter_Echo    3

#define  Signaal_Ledjes                  23
#define  Volgmodus_Autonoom_Knop         46      //Define hier de pin voor de autonome knop

//Overige Pinnen
#define SDA_Pin                         20
#define SCL_Pin                         21
#define Shut_ToF_Rechts                 50
#define Shut_ToF_Links                  51
#define Signaal_Ledjes                  23

//Time of Flight Adressen
#define address0 0x20
#define address1 0x22

//Constanten Defines en Variabelen
#define STEPS                 200
#define Motor_Speed_Max       120      
#define  Motor_Speed_Follow    (Motor_Speed_Max/0.7)   //70% speed voor het volgen van een persoon                 
#define Motor_Speed_Stop      0    

#define RANGE 1
#define Rechts_Afslaan        1
#define Links_Afslaan         -1

#define Rij_Snelheid          512
#define Arm_Lengte            87,5  //milimeter 1/8 Schaalmodel van 0,7 Meter
#define Gewas_Afstand         50    //afstand van de gewassen tot de agv (misschien iets meer)
#define Koers_Value           70    //afstand van de rand van de gewassen tot agv
#define Koers_Marge           30    //hoeveelheid speelruimte van de koers van de agv

//#define Bocht0
#define Linksom               -1
#define Rechtsom              1
