#include "highscore.h"

void InitHighScoreTexts(struct GameData* _gd)
{
	_gd->highScore.text[OUTLINE] = LoadText(_gd->font.outline, 90);
	_gd->highScore.text[NORMAL] = LoadText(_gd->font.normal, 50);
}

void SortHighScores(struct GameData* _gd)
{
	for (int k = 0; k < _gd->highScore.nb; k++)
	{
		for (int i = 0; i < _gd->highScore.nb - 1; i++)
		{
			if (_gd->highScore.tab[i].score < _gd->highScore.tab[i + 1].score)
			{
				struct OneHighScore highScore = _gd->highScore.tab[i];
				_gd->highScore.tab[i] = _gd->highScore.tab[i + 1];
				_gd->highScore.tab[i + 1] = highScore;
			}
		}
	}
}

void ReadHighScores(struct GameData* _gd)
{
	FILE* ptrFile;

	if (!(ptrFile = fopen("../HighScores.bin", "rb")))
	{
		printf("Error file acces\n");
		exit(EXIT_FAILURE);
	}

	fread(&_gd->highScore, sizeof(struct HighScore), 1, ptrFile);

	fclose(ptrFile);
}

void SaveHighScore(struct GameData* _gd)
{
	FILE* ptrFile;
	time_t rawtime;
	struct tm* timeinfo;
	struct OneHighScore* highScore = &_gd->highScore.tab[_gd->highScore.nb];

	if (!(ptrFile = fopen("../HighScores.bin", "wb")))
	{
		printf("Error file acces\n");
		exit(EXIT_FAILURE);
	}

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	highScore->score = _gd->player.score;
	printf("Enter your name :\n");
	scanf("%s", highScore->name);
	system("cls");
	strftime(highScore->date, 50, "%Y/%m/%d %H:%M:%S", timeinfo);

	if (_gd->highScore.nb < NB_HIGHSCORE_MAX)
	{
		_gd->highScore.nb++;
		fwrite(&_gd->highScore, sizeof(struct HighScore), 1, ptrFile);
	}

	fclose(ptrFile);

	InitHighScoreTexts(_gd);
	SortHighScores(_gd);
}

void DisplayHighScores(struct GameData* _gd)
{
	struct HighScore* highScore = &_gd->highScore;

	BlitText(highScore->text[OUTLINE], (sfVector2f) { 50, 50 }, "Rank", _gd->syst.window);
	BlitText(highScore->text[OUTLINE], (sfVector2f) { 400, 50 }, "Name", _gd->syst.window);
	BlitText(highScore->text[OUTLINE], (sfVector2f) { 900, 50 }, "Date", _gd->syst.window);
	BlitText(highScore->text[OUTLINE], (sfVector2f) { 1325, 50 }, "Score", _gd->syst.window);

	for (int i = 0; i < 10; i++)
	{
		char bufferNb[5];
		char bufferName[50];
		char bufferTime[50];
		char bufferScore[50];

		sprintf(bufferNb, "%d", i + 1);
		sprintf(bufferName, "%s", highScore->tab[i].name);
		sprintf(bufferTime, "%s", highScore->tab[i].date);
		sprintf(bufferScore, "%d", highScore->tab[i].score);

		BlitText(highScore->text[NORMAL], (sfVector2f) { 125, 175 + (float)(70 * i) }, bufferNb, _gd->syst.window);
		BlitText(highScore->text[NORMAL], (sfVector2f) { 400, 175 + (float)(70 * i) }, bufferName, _gd->syst.window);
		BlitText(highScore->text[NORMAL], (sfVector2f) { 800, 175 + (float)(70 * i) }, bufferTime, _gd->syst.window);
		BlitText(highScore->text[NORMAL], (sfVector2f) { 1375, 175 + (float)(70 * i) }, bufferScore, _gd->syst.window);
	}
}