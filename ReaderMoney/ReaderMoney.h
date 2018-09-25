#pragma once


class ReaderMoney
{
	const int MAX_LIMIT_FOR_MONEY = 1000000;	// Заданный лимит в миллион
	std::map<std::string, int> mapStrToInt;		// Сопоставление письменному значению его численный эквивалент 

	std::vector<std::string> wordsOfMoney;		// Отдельные слова поданной строки
	int counterOfRubles;
	int counterOfKopecks;

public:
	ReaderMoney();

	std::pair<bool, std::pair<int, int>> getMoney(std::string lineWithMoney);	// Перевод количества денег в денежную сумму

	int getRubles() { return counterOfRubles; }
	int getKopecks() { return counterOfKopecks; }

private:
	void splitToWords(std::string line);		// Деление строки на отдельные слова

	int countOfKopecks();						// Подсчет копеек
	int countOfRubles();						// Подсчет рублей

	int findPosWithRubles();					// Проверка, что в строке записаны рубли и возврат позицию соответствующего слова (или -1)

	bool stringToNumbers(std::string word);		// Перевод строки с числом в само число

	void preparationForWork()					// Очищение всех предыдущих значений
	{
		counterOfRubles = 0;
		counterOfKopecks = 0;
		wordsOfMoney.clear();
	}
};