#pragma once
#include <QString>

class Recongnizer
{
public:
	Recongnizer();
	~Recongnizer();

	QString DoRecongnize(const char* data, int size);
};

