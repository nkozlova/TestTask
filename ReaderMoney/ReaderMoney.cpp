#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include "ReaderMoney.h"


int main()
{
	setlocale(LC_ALL, "Russian");

	ReaderMoney reader;
	
	reader.getMoney("Одна тысяча девятьсот рублей 00 коп");
	reader.getMoney("15 коп");
	reader.getMoney("Пятнадцать рублей");
	reader.getMoney("Сто двадцать рублей 70 коп");

	return 0;
}

std::pair<bool, std::pair<int, int>> ReaderMoney::getMoney(std::string lineWithMoney)
// На вход подается строка, содержащая денежную сумму прописью
// На выход - флаг, удалось ли распарсить строку
{
	//	Обнуление всех значений
	preparationForWork();

	// Разделение строки на слова по пробелам
	splitToWords(lineWithMoney);

	if (wordsOfMoney.size() < 2)
	{
		return std::pair<bool, std::pair<int, int>>(false, std::pair<int, int>(0, 0));
	}

	if (countOfRubles() < 0 || countOfKopecks() < 0)
	{
		std::cout << "Invalid data" << std::endl;
		return std::pair<bool, std::pair<int, int>>(false, std::pair<int, int>(0, 0));
	}

	std::cout << counterOfRubles << " руб, " << counterOfKopecks << " коп" << std::endl;

	return std::pair<bool, std::pair<int, int>>(true, std::pair<int, int>(counterOfRubles, counterOfKopecks));
}

void ReaderMoney::splitToWords(std::string line)
{
	// Приводим все буквы к нижнему регистру
	std::transform(line.begin(), line.end(), line.begin(), tolower);

	std::stringstream data(line);

	std::string word;
	while (std::getline(data, word, ' '))
	{
		if (!word.empty())
		{
			wordsOfMoney.push_back(word);
		}
	}
}

int ReaderMoney::countOfRubles()
{
	// Поиск позиции слова <руб>
	int pos = findPosWithRubles();

	if (pos < 0)
	{
		// не нашли рублей
		return 0;
	}

	if (pos == 0 && wordsOfMoney[0] == "рубль")
	{
		counterOfRubles = 1;
	}

	for (int i = 0; i < pos; i++)
	{
		if (!stringToNumbers(wordsOfMoney[i]))
		{
			// что-то пошло не так (неправильня запись)
			return -1;
		}

		// Превышение лимита
		if (counterOfRubles >= MAX_LIMIT_FOR_MONEY)
		{
			return -1;
		}
	}

	return counterOfRubles;
}

int ReaderMoney::countOfKopecks()
{
	// Если копейки есть, они всегда занимают два последних сллова строки
	std::string word = wordsOfMoney[wordsOfMoney.size() - 1];
	std::string pattern = "коп";
	if (word.find(pattern) != std::string::npos)
	{
		// Если на месте копеек мы прочитали не только цифры, то есть строка неформатна 
		if (wordsOfMoney[wordsOfMoney.size() - 2].find_first_not_of("0123456789") != std::string::npos)
		{
			return -1;
		}

		counterOfKopecks = atoi(wordsOfMoney[wordsOfMoney.size() - 2].c_str());

		// На месте коппек могут быть и рубли, например, 150 копеек
		if (counterOfKopecks > 99)
		{
			counterOfRubles += counterOfKopecks / 100;
			counterOfKopecks = counterOfKopecks % 100;
		}

		return counterOfKopecks;
	}
	
	// Копейки вообще не вводились	
	return 0;
}

int ReaderMoney::findPosWithRubles()
// Слово <руб> может быть либо последним в строке, либо третьим с конца, если еще записаны копейки
{
	// Останется -1, если рули не вводились
	int pos = -1;

	std::string pattern = "руб";
	if (wordsOfMoney[wordsOfMoney.size() - 1].find(pattern) != std::string::npos)
	{
		pos = wordsOfMoney.size() - 1;
	}
	else if (wordsOfMoney.size() >= 3 && wordsOfMoney[wordsOfMoney.size() - 3].find(pattern) != std::string::npos)
	{
		pos = wordsOfMoney.size() - 3;
	}

	return pos;
}

bool ReaderMoney::stringToNumbers(std::string word)	// Преобразование слова в число
// Возвращает флаг, удалось ли преобразование
{
	if (mapStrToInt.find(word) != mapStrToInt.end())
	{
		counterOfRubles += mapStrToInt[word];
		return true;
	} 
	else if (word.find("тысяч") != std::string::npos)
	{
		if (counterOfRubles > 0) {
			counterOfRubles *= 1000;
		}
		else
		{
			counterOfRubles = 1000;
		}
		return true;
	}

	return false;
}

ReaderMoney::ReaderMoney() : counterOfRubles(0), counterOfKopecks(0)
{
	mapStrToInt.insert(std::pair<std::string, int>("девятьсот", 900));
	mapStrToInt.insert(std::pair<std::string, int>("восемьсот", 800));
	mapStrToInt.insert(std::pair<std::string, int>("семьсот", 700));
	mapStrToInt.insert(std::pair<std::string, int>("шестьсот", 600));
	mapStrToInt.insert(std::pair<std::string, int>("пятьсот", 500));
	mapStrToInt.insert(std::pair<std::string, int>("четыреста", 400));
	mapStrToInt.insert(std::pair<std::string, int>("триста", 300));
	mapStrToInt.insert(std::pair<std::string, int>("двести", 200));
	mapStrToInt.insert(std::pair<std::string, int>("сто", 100));

	mapStrToInt.insert(std::pair<std::string, int>("девяносто", 90));
	mapStrToInt.insert(std::pair<std::string, int>("восемьдесят", 80));
	mapStrToInt.insert(std::pair<std::string, int>("семьдесят", 70));
	mapStrToInt.insert(std::pair<std::string, int>("шестьдесят", 60));
	mapStrToInt.insert(std::pair<std::string, int>("пятьдесят", 50));
	mapStrToInt.insert(std::pair<std::string, int>("сорок", 40));
	mapStrToInt.insert(std::pair<std::string, int>("тридцать", 30));
	mapStrToInt.insert(std::pair<std::string, int>("двадцать", 20));
	mapStrToInt.insert(std::pair<std::string, int>("десять", 10));

	mapStrToInt.insert(std::pair<std::string, int>("девятнадцать", 19));
	mapStrToInt.insert(std::pair<std::string, int>("восемнадцать", 18));
	mapStrToInt.insert(std::pair<std::string, int>("семнадцать", 17));
	mapStrToInt.insert(std::pair<std::string, int>("шестнадцать", 16));
	mapStrToInt.insert(std::pair<std::string, int>("пятнадцать", 15));
	mapStrToInt.insert(std::pair<std::string, int>("четырнадцать", 14));
	mapStrToInt.insert(std::pair<std::string, int>("тринадцать", 13));
	mapStrToInt.insert(std::pair<std::string, int>("двенадцать", 12));
	mapStrToInt.insert(std::pair<std::string, int>("одиннадцать", 11));

	mapStrToInt.insert(std::pair<std::string, int>("девять", 9));
	mapStrToInt.insert(std::pair<std::string, int>("восемь", 8));
	mapStrToInt.insert(std::pair<std::string, int>("семь", 7));
	mapStrToInt.insert(std::pair<std::string, int>("шесть", 6));
	mapStrToInt.insert(std::pair<std::string, int>("пять", 5));
	mapStrToInt.insert(std::pair<std::string, int>("четыре", 4));
	mapStrToInt.insert(std::pair<std::string, int>("три", 3));
	mapStrToInt.insert(std::pair<std::string, int>("два", 2));
	mapStrToInt.insert(std::pair<std::string, int>("две", 2));
	mapStrToInt.insert(std::pair<std::string, int>("один", 1));
	mapStrToInt.insert(std::pair<std::string, int>("одна", 1));
	mapStrToInt.insert(std::pair<std::string, int>("ноль", 0));
}
