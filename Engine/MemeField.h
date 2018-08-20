#pragma once

#include "Graphics.h"
#include "Sound.h"

class MemeField
{
public:
	enum class State
	{
		Lost,
		Won,
		Playing
	};
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Flagged,
			Revealed
		};
	public:
		void SpawnMeme();
		bool HasMeme() const;
		void Draw( const Vei2& screenPos, MemeField::State fieldState, Graphics& gfx ) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		void SetNeighborMemeCount( int memeCount );
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighborMemes = -1;
	};
public:

	

	MemeField( int nMemes );
	void Draw( Graphics& gfx ) const;
	RectI GetRect() const;
	void OnRevealClick( const Vei2& screenPos );
	void OnFlagClick( const Vei2& screenPos );
	bool IsGameWon() const;
	bool IsGameLost() const;
	State GetState() const;
private:
	Tile& TileAt( const Vei2& gridPos );
	const Tile& TileAt( const Vei2& gridPos ) const;
	Vei2 ScreenToGrid( const Vei2& screenPos );
	int CountNeighborMemes( const Vei2& gridPos );
	
private:
	static constexpr int width = 3;
	static constexpr int height = 3;
	State gameState = State::Playing;
	Tile field[width * height];
	const Vei2 offset = { 220, 200 }; 
	static constexpr int borderWidth = 20;
	Sound soundLose = Sound( L"spayed.wav");
};