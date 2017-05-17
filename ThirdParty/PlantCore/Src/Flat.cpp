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

Flat::Flat()
{
	Machine caseta("Caseta");
	for (size_t i = 0; i < 82; i++)
	{
		std::size_t found = machineList[i].find("Caseta");
		if (found != std::string::npos)
		{
			MachinePart p(machineList[i], materialList[i]);
			caseta.machineParts.push_back(p);
		}
		else
		{
			Machine m(machineList[i]);
			MachinePart p(machineList[i], materialList[i]);
			m.machineParts.push_back(p);
			machines.push_back(m);
		}
		
	}
	machines.push_back(caseta);
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
