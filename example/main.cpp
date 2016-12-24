#include "bitwise_enum.h"
#include <iostream>

class Widget {
public:
	enum class Alignment{
		none = 0,
		top = 1,
		bottom = 2,
		left = 4,
		right = 8,
	} alignment;
};

// Enable bitwise operators for enumeration
ENABLE_BITWISE_OPERATORS(Widget::Alignment);

int main(int argc, char **argv){
	Widget::Alignment align = Widget::Alignment::none;
	align |= Widget::Alignment::top;
	align |= Widget::Alignment::left;

	if (bool(align & Widget::Alignment::top))
		std::cout << "Aligning top ...\n";
	if (bool(align & Widget::Alignment::bottom))
		std::cout << "Aligning bottom ...\n";
	if (bool(align & Widget::Alignment::left))
		std::cout << "Aligning left ...\n";
	if (bool(align & Widget::Alignment::right))
		std::cout << "Aligning right ...\n";

	return 0;
}
