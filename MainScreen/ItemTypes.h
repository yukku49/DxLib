#pragma once

enum Item_number
{
	TOMATO,
	BASIL,
	CHEESE,
	GORGONZOLA,
	PIZZADOUGH,
	ITEM_MAX
};

enum class PizzaType
{
	None,
	Margherita,
	QuattroFormaggi,
	Marinara,
	Genovese,
	Sage
};

struct Item_count
{
	int Pizzadough_Counter = 0;
	int Tmato_Counter = 0;
	int Cheese_Counter = 0;
	int Gorgonzola_Counter = 0;
	int Basil_Counter = 0;
};

struct PizzaTimer
{
	float Marigherita = 0.0f;
	float QuattroFormaggi = 0.0f;
	float Genovese = 0.0f;
	float Marinara = 0.0f;
};
