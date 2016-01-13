#ifndef WEIGHT_H
#define WEIGHT_H
#endif
class Weights
{
private:
	int index;
	float weight;
public:
	Weights();
	void setIndex(int i) { index = i; }
	void setWeight(float w) { weight = w; }
	int getIndex() { return index; }
	float getWeight() { return weight; }
	~Weights();
};

