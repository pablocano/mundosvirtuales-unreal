#include "Flat.h"
#include <string>


std::string Flat::machineList[82] = { "/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_B4_TRed.Planta-05102017_Bombas_y_tuberias__Accesorios_B4_TRed",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_B5_codo.Planta-05102017_Bombas_y_tuberias__Accesorios_B5_codo",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_B6.Planta-05102017_Bombas_y_tuberias__Accesorios_B6",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_C1.Planta-05102017_Bombas_y_tuberias__Accesorios_C1",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_C2.Planta-05102017_Bombas_y_tuberias__Accesorios_C2",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_D1_golilla.Planta-05102017_Bombas_y_tuberias__Accesorios_D1_golilla",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_D1_perno.Planta-05102017_Bombas_y_tuberias__Accesorios_D1_perno",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_D1_tuerca.Planta-05102017_Bombas_y_tuberias__Accesorios_D1_tuerca",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_E1.Planta-05102017_Bombas_y_tuberias__Accesorios_E1",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Accesorios_Union.Planta-05102017_Bombas_y_tuberias__Accesorios_Union",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_10.Planta-05102017_Bombas_y_tuberias__Bomba_10",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_11.Planta-05102017_Bombas_y_tuberias__Bomba_11",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_12.Planta-05102017_Bombas_y_tuberias__Bomba_12",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_13.Planta-05102017_Bombas_y_tuberias__Bomba_13",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_14.Planta-05102017_Bombas_y_tuberias__Bomba_14",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_15.Planta-05102017_Bombas_y_tuberias__Bomba_15",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_01.Planta-05102017_Bombas_y_tuberias__Bomba_01",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_02.Planta-05102017_Bombas_y_tuberias__Bomba_02",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_03.Planta-05102017_Bombas_y_tuberias__Bomba_03",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_04.Planta-05102017_Bombas_y_tuberias__Bomba_04",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_05_1.Planta-05102017_Bombas_y_tuberias__Bomba_05_1",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_05_2.Planta-05102017_Bombas_y_tuberias__Bomba_05_2",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_06.Planta-05102017_Bombas_y_tuberias__Bomba_06",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_07.Planta-05102017_Bombas_y_tuberias__Bomba_07",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_08.Planta-05102017_Bombas_y_tuberias__Bomba_08",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_09.Planta-05102017_Bombas_y_tuberias__Bomba_09",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_16_esparrago.Planta-05102017_Bombas_y_tuberias__Bomba_16_esparrago",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_16_golilla.Planta-05102017_Bombas_y_tuberias__Bomba_16_golilla",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_16_tuerca.Planta-05102017_Bombas_y_tuberias__Bomba_16_tuerca",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_17_esparrago.Planta-05102017_Bombas_y_tuberias__Bomba_17_esparrago",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_17_golilla.Planta-05102017_Bombas_y_tuberias__Bomba_17_golilla",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Bomba_17_tuerca.Planta-05102017_Bombas_y_tuberias__Bomba_17_tuerca",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Caja_recolectora.Planta-05102017_Bombas_y_tuberias__Caja_recolectora",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Estanque_600_CR_002.Planta-05102017_Bombas_y_tuberias__Estanque_600_CR_002",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Estanque_600_TK_001.Planta-05102017_Bombas_y_tuberias__Estanque_600_TK_001",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Filtro.Planta-05102017_Bombas_y_tuberias__Filtro",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Filtro_tapa.Planta-05102017_Bombas_y_tuberias__Filtro_tapa",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Golilla.Planta-05102017_Bombas_y_tuberias__Golilla",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Motor.Planta-05102017_Bombas_y_tuberias__Motor",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Perno.Planta-05102017_Bombas_y_tuberias__Perno",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Reduccion_bomba.Planta-05102017_Bombas_y_tuberias__Reduccion_bomba",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador.Planta-05102017_Bombas_y_tuberias__Soplador",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_bomba.Planta-05102017_Bombas_y_tuberias__Soplador_bomba",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_disco.Planta-05102017_Bombas_y_tuberias__Soplador_disco",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_estanque.Planta-05102017_Bombas_y_tuberias__Soplador_estanque",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_golilla.Planta-05102017_Bombas_y_tuberias__Soplador_golilla",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_motor.Planta-05102017_Bombas_y_tuberias__Soplador_motor",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_perno.Planta-05102017_Bombas_y_tuberias__Soplador_perno",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_proteccion.Planta-05102017_Bombas_y_tuberias__Soplador_proteccion",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soplador_tuerca.Planta-05102017_Bombas_y_tuberias__Soplador_tuerca",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Soporte.Planta-05102017_Bombas_y_tuberias__Soporte",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Terreno.Planta-05102017_Bombas_y_tuberias__Terreno",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Tuberia_A1.Planta-05102017_Bombas_y_tuberias__Tuberia_A1",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Tuberia_A2.Planta-05102017_Bombas_y_tuberias__Tuberia_A2",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Tuberia_A3.Planta-05102017_Bombas_y_tuberias__Tuberia_A3",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Tuberia_A6.Planta-05102017_Bombas_y_tuberias__Tuberia_A6",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Tuberia_A7.Planta-05102017_Bombas_y_tuberias__Tuberia_A7",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Tuerca.Planta-05102017_Bombas_y_tuberias__Tuerca",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas.Planta-05102017_Bombas_y_tuberias__Valvulas",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas_bola.Planta-05102017_Bombas_y_tuberias__Valvulas_bola",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas_golillas.Planta-05102017_Bombas_y_tuberias__Valvulas_golillas",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas_manilla.Planta-05102017_Bombas_y_tuberias__Valvulas_manilla",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas_pernos.Planta-05102017_Bombas_y_tuberias__Valvulas_pernos",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas_placa.Planta-05102017_Bombas_y_tuberias__Valvulas_placa",
"/Game/Proter/Planta-05102017_Bombas_y_tuberias__Valvulas_tuercas.Planta-05102017_Bombas_y_tuberias__Valvulas_tuercas",
"/Game/Proter/Planta-05102017_Caseta__Barandas_Caseta.Planta-05102017_Caseta__Barandas_Caseta",
"/Game/Proter/Planta-05102017_Caseta__Bases.Planta-05102017_Caseta__Bases",
"/Game/Proter/Planta-05102017_Caseta__Container.Planta-05102017_Caseta__Container",
"/Game/Proter/Planta-05102017_Caseta__Cristal.Planta-05102017_Caseta__Cristal",
"/Game/Proter/Planta-05102017_Caseta__Estructura_escalera.Planta-05102017_Caseta__Estructura_escalera",
"/Game/Proter/Planta-05102017_Caseta__Manillas.Planta-05102017_Caseta__Manillas",
"/Game/Proter/Planta-05102017_Caseta__Marcos.Planta-05102017_Caseta__Marcos",
"/Game/Proter/Planta-05102017_Caseta__Piso_escalera.Planta-05102017_Caseta__Piso_escalera",
"/Game/Proter/Planta-05102017_Caseta__Puertas.Planta-05102017_Caseta__Puertas",
"/Game/Proter/Planta-05102017_Caseta__Techo.Planta-05102017_Caseta__Techo",
"/Game/Proter/Planta-05102017_Linea_Electrica__Bases_linea_electrica.Planta-05102017_Linea_Electrica__Bases_linea_electrica",
"/Game/Proter/Planta-05102017_Linea_Electrica__Canaletas.Planta-05102017_Linea_Electrica__Canaletas",
"/Game/Proter/Planta-05102017_Planta__ARRIOSTRAM.Planta-05102017_Planta__ARRIOSTRAM",
"/Game/Proter/Planta-05102017_Planta__Barandas_Planta.Planta-05102017_Planta__Barandas_Planta",
"/Game/Proter/Planta-05102017_Planta__CIELO.Planta-05102017_Planta__CIELO",
"/Game/Proter/Planta-05102017_Planta__Estructura_Planta.Planta-05102017_Planta__Estructura_Planta",
"/Game/Proter/Planta-05102017_Planta__PISO.Planta-05102017_Planta__PISO" };

std::string Flat::materialList[82] = { "/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/YellowMetal.YellowMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/YellowMetal.YellowMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/GreenPlastic.GreenPlastic",
"/Game/Materials/ProterMaterials/GreenPlastic.GreenPlastic",
"/Game/Materials/ProterMaterials/GreenPlastic.GreenPlastic",
"/Game/Materials/ProterMaterials/CyanMetal.CyanMetal",
"/Game/Materials/ProterMaterials/CyanMetal.CyanMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/BlueMetal.BlueMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/BlueMetal.BlueMetal",
"/Game/Materials/ProterMaterials/BlueMetal.BlueMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/BlueMetal.BlueMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/BlueMetal.BlueMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/OrangeMetal.OrangeMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/StarterContent/Materials/M_Concrete_Panels.M_Concrete_Panels",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/GoldMetal.GoldMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/RedMetal.RedMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/DarkGrayMetal.DarkGrayMetal",
"/Game/Materials/ProterMaterials/YellowMetal.YellowMetal",
"/Game/StarterContent/Materials/M_Concrete_Tiles.M_Concrete_Tiles",
"/Game/Materials/ProterMaterials/OrangeMetal.OrangeMetal",
"/Game/StarterContent/Materials/M_Glass.M_Glass",
"/Game/Materials/ProterMaterials/BlueMetal.BlueMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/YellowMetal.YellowMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/BrownPlastic.BrownPlastic",
"/Game/Materials/ProterMaterials/BlueMetal.BlueMetal",
"/Game/Materials/ProterMaterials/YellowMetal.YellowMetal",
"/Game/Materials/ProterMaterials/GrayMetal.GrayMetal",
"/Game/Materials/ProterMaterials/BlueMetal.BlueMetal",
"/Game/Materials/ProterMaterials/YellowMetal.YellowMetal"
};

std::string Flat::machineInfo[82] = { 
	"Accesorios B4 TRed",
	"Accesorios B5 codo",
	"Accesorios B6",
	"Accesorios C1",
	"Accesorios C2",
	"Accesorios D1 golilla",
	"Accesorios D1 perno",
	"Accesorios D1 tuerca",
	"Accesorios E1",
	"Accesorios Union",
	"Flange Giratorio 2",
	"Descarga",
	"Caja Impulsor",
	"Flange de Amarre",
	"Descarga",
	"Pasador Impulsor",
	"Soporte Motor",
	"Separador",
	"Machón de Acople",
	"Seguro de Sello",
	"Sello Plano M50 1",
	"Sello Plano M50 2",
	"Tapa Impulsión",
	"Alma Metálica",
	"Pasador Impulsor",
	"Impulsor",
	"Esparrago - Espárrago 3/8",
	"Golilla",
	"Tuerca",
	"Espárrago 1/2",
	"Golilla",
	"Tuerca",
	"Caja Recolectora",
	"Estanque 600 CR 002",
	"Estanque 600 TK 001",
	"Filtro",
	"Filtro tapa",
	"Golilla",
	"MOTOR ELECTRICO\n \
	Tipo: Trifásico\n \
	Clase : IE2 - 225M - 4\n \
	Potencia : 4,1 kw(5.5 Hp)\n \
	RPM : 1470\n \
	Consumo : 8.1 Amp\n \
	Eficiencia : 86.6%\n \
	Factor de Potencia : 0.81",
	"Perno",
	"Reduccion Bomba",
	"Soplador",
	"Soplador Bomba",
	"Soplador Disco",
	"Soplador Estanque",
	"Soplador Golilla",
	"Soplador Motor",
	"Soplador perno",
	"Soplador proteccion",
	"Soplador tuerca",
	"Soporte",
	"Terreno",
	"Tuberia A1",
	"Tuberia A2",
	"Tuberia A3",
	"Tuberia A6",
	"Tuberia A7",
	"Tuerca",
	"Valvulas",
	"VALVULA BOLA\n \
	Clase: 150\n \
	Bola Acero Inoxidable\n \
	Asientos PTFE\n \
	Para Instalación Vertical u Horizontal\n \
	Asientos recambiables\n \
	Extremos flangeados\n \
	Peso(aprox) 17 kg\n \
	",
	"Valvulas golillas",
	"Valvulas manilla",
	"Valvulas pernos",
	"Valvulas placa",
	"Valvulas tuercas",
	"Barandas Caseta",
	"Bases",
	"Container",
	"Cristal",
	"Estructura escalera",
	"Manillas",
	"Marcos",
	"Piso escalera",
	"Puertas",
	"Techo",
	"Bases linea electrica",
	"Canaletas",
	"ARRIOSTRAM",
	"Barandas Planta",
	"CIELO",
	"Estructura Planta",
	"PISO"
};
std::string Flat::machineShortInfo[82] = {
	"Accesorios B4 TRed",
	"Accesorios B5 codo",
	"Accesorios B6",
	"Accesorios C1",
	"Accesorios C2",
	"Accesorios D1 golilla",
	"Accesorios D1 perno",
	"Accesorios D1 tuerca",
	"Accesorios E1",
	"Accesorios Union",
	"Flange Giratorio 2",
	"Descarga",
	"Caja Impulsor",
	"Flange de Amarre",
	"Descarga",
	"Pasador Impulsor",
	"Soporte Motor",
	"Separador",
	"Machón de Acople",
	"Seguro de Sello",
	"Sello Plano M50 1",
	"Sello Plano M50 2",
	"Tapa Impulsión",
	"Alma Metálica",
	"Pasador Impulsor",
	"Impulsor",
	"Esparrago 3/8",
	"Golilla",
	"Tuerca",
	"Esparrago 1/2",
	"Golilla",
	"Tuerca",
	"Caja Recolectora",
	"Estanque 600 CR 002",
	"Estanque 600 TK 001",
	"Filtro",
	"Filtro tapa",
	"Golilla",
	"Motor",
	"Perno",
	"Reduccion Bomba",
	"Soplador",
	"Soplador Bomba",
	"Soplador Disco",
	"Soplador Estanque",
	"Soplador Golilla",
	"Soplador Motor",
	"Soplador perno",
	"Soplador proteccion",
	"Soplador tuerca",
	"Soporte",
	"Terreno",
	"Tuberia A1",
	"Tuberia A2",
	"Tuberia A3",
	"Tuberia A6",
	"Tuberia A7",
	"Tuerca",
	"Valvulas",
	"Valvulas bola",
	"Valvulas golillas",
	"Valvulas manilla",
	"Valvulas pernos",
	"Valvulas placa",
	"Valvulas tuercas",
	"Barandas Caseta",
	"Bases",
	"Container",
	"Cristal",
	"Estructura escalera",
	"Manillas",
	"Marcos",
	"Piso escalera",
	"Puertas",
	"Techo",
	"Bases linea electrica",
	"Canaletas",
	"ARRIOSTRAM",
	"Barandas Planta",
	"CIELO",
	"Estructura Planta",
	"PISO"
};


std::string Flat::machinePartNumber[82] = {
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"PR1-10",
	"PR1-11",
	"PR1-12",
	"PR1-13",
	"PR1-14",
	"PR1-15",
	"PR1-01",
	"PR1-02",
	"PR1-03",
	"PR1-04",
	"PR1-05 1",
	"PR1-05 2",
	"PR1-06",
	"PR1-07",
	"PR1-08",
	"PR1-09",
	"PR1-16",
	"PR1-17",
	"PR1-17",
	"PR1-17",
	"PR1-17",
	"PR1-17",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"PR0101-1",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"PR1-1", // Soporte Motor
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA",
	"NA"
};

Flat::Flat()
{
	std::string bombaInfo = "BOMBA\n \
		Fabricante: Reifox\n \
		Tipo : centrifuga horizontal\n \
		Modelo : 1 - 210 - 5.5 hp 1470 RPM\n \
		Q : 55 mt3 / h\n \
		H : 12 mca";
	Machine caseta("Caseta", "Nada de info", "Caseta",  "NA", true);
	Machine bomba("Bomba", bombaInfo, "Bomba", "PR0501-3", true,true);
	Machine soplador("Soplador", "Nada de Info", "Soplador","NA", true);
	Machine planta("Planta", "", "Planta","NA", true);
	Machine *machine;

	for (size_t i = 0; i < 82; i++)
	{

		if (machineList[i].find("Bombas_y_tuberias__Bomba") != std::string::npos ||
			machineList[i].find("Bombas_y_tuberias__Reduccion_bomba") != std::string::npos ||
			machineList[i].find("Bombas_y_tuberias__Motor") != std::string::npos)
			machine = &bomba;
		else if (machineList[i].find("Bombas_y_tuberias__Soplador") != std::string::npos)
			machine = &soplador;
		else if (machineList[i].find("Caseta") != std::string::npos && machineList[i].find("Bases") == std::string::npos)
		{
			machine = &caseta;
		}
		else if (machineList[i].find("_Planta__") != std::string::npos)
		{
			machine = &planta;
		}
		else
		{
			Machine m(machineList[i], machineInfo[i], machineShortInfo[i], machinePartNumber[i]);
			machines.push_back(m);
			machine = &machines.back();
		}

		MachinePart p(machineList[i], materialList[i], machineShortInfo[i], machineInfo[i], machinePartNumber[i]);
		machine->machineParts.push_back(p);
		
	}
	machines.push_back(caseta);
	machines.push_back(bomba);
	machines.push_back(soplador);
	machines.push_back(planta);
	/*Machine wheel("wheel");

	for (int i = 2; i < 36; i++)
	{
		if (i < 10)
		{
			MachinePart p("00" + std::to_string(i));
			wheel.machineParts.push_back(p);
		}
		else {
			MachinePart p("0" + std::to_string(i));
			wheel.machineParts.push_back(p);
		}	
	}*/

	//machines.push_back(wheel);
}


Flat::~Flat()
{
}
