#include "xpch.h"
#include "ChessPiece.h"

std::vector<BoardPosition> Pawn::computePossibleMoves()
{
	std::vector<BoardPosition> possibleMoves;

	int direction = (m_color == PieceColor::white) ? -1 : 1;
	auto [r, c] = this->getPosition();

	int doubleColumns[2] = {1,6};
	

	if (m_boardScript->m_board[r + direction][c] == nullptr)
	{
		possibleMoves.push_back(BoardPosition(r+direction,c));
		if (r == doubleColumns[(int)this->getColor()]) {
			if (m_boardScript->m_board[r + 2 * direction][c] == nullptr)
			{
				possibleMoves.push_back(BoardPosition(r + 2 * direction, c));
			}
		}

	}
	
	//left diagnol
	if((direction == -1 && r > 0) || (direction == 1 && r < 7))
	if(c > 0 && m_boardScript->m_board[r + direction][c - 1] != nullptr)
		if (m_boardScript->m_board[r + direction][c - 1]->getColor() != m_color)
		{
			possibleMoves.push_back(BoardPosition(r + direction, c - 1));
		}


	//right diagnol
	if ((direction == -1 && r > 0) || (direction == 1 && r < 7))
	if(c < 7 && m_boardScript->m_board[r + direction][c + 1] != nullptr)
		if (m_boardScript->m_board[r + direction][c+1]->getColor() != m_color)
		{
			possibleMoves.push_back(BoardPosition(r + direction, c + 1));
		}

	
	return possibleMoves;
}

std::vector<BoardPosition> Knight::computePossibleMoves()
{
	std::vector<BoardPosition> possibleMoves;
	auto [r, c] = this->getPosition();

	
	if (r > 1) { //go up
		if (c > 0) { //go left
			if (this->m_boardScript->m_board[r - 2][c - 1] == nullptr || this->m_boardScript->m_board[r - 2][c - 1]->getColor() != this->getColor())
				possibleMoves.push_back({ r - 2,c - 1 });
		}
		if (c < 7) { //go right
			if (this->m_boardScript->m_board[r - 2][c + 1] == nullptr || this->m_boardScript->m_board[r - 2][c + 1]->getColor() != this->getColor())
				possibleMoves.push_back({ r - 2,c + 1 });
		}
	}

	if (r < 6) { //go down
		if (c > 0) { //go left
			if (this->m_boardScript->m_board[r + 2][c - 1] == nullptr || this->m_boardScript->m_board[r + 2][c - 1]->getColor() != this->getColor())
				possibleMoves.push_back({ r + 2,c - 1 });
		}
		if (c < 7) { //go right
			if (this->m_boardScript->m_board[r + 2][c + 1] == nullptr || this->m_boardScript->m_board[r + 2][c + 1]->getColor() != this->getColor())
				possibleMoves.push_back({ r + 2,c + 1 });
		}
	}

	if (c > 1) { //go left
		if (r > 0) { //go up
			if (this->m_boardScript->m_board[r - 1][c - 2] == nullptr || this->m_boardScript->m_board[r - 1][c - 2]->getColor() != this->getColor())
				possibleMoves.push_back({ r - 1,c - 2 });
		}
		if (r < 7) { //go down
			if (this->m_boardScript->m_board[r + 1][c - 2] == nullptr || this->m_boardScript->m_board[r + 1][c - 2]->getColor() != this->getColor())
				possibleMoves.push_back({ r + 1,c - 2 });
		}
	}

	if (c < 6) { //go right
		if (r > 0) { //go up
			if (this->m_boardScript->m_board[r - 1][c + 2] == nullptr || this->m_boardScript->m_board[r - 1][c + 2]->getColor() != this->getColor())
				possibleMoves.push_back({ r - 1,c + 2 });
		}
		if (r < 7) { //go down
			if (this->m_boardScript->m_board[r + 1][c + 2] == nullptr || this->m_boardScript->m_board[r + 1][c + 2]->getColor() != this->getColor())
				possibleMoves.push_back({ r + 1,c + 2 });
		}
	}

	return possibleMoves;
}

std::vector<BoardPosition> Rook::computePossibleMoves()
{
	std::vector<BoardPosition> possibleMoves;
	auto [r, c] = this->getPosition();

	//check up
	for (int row = r-1; row >= 0; row--)
	{
		if (this->m_boardScript->m_board[row][c]) {
			//cannot capture color
			if (this->m_boardScript->m_board[row][c]->getColor() == this->m_color)
				break;
			//can capture
			if (this->m_boardScript->m_board[row][c]->getColor() != this->m_color)
			{
				possibleMoves.push_back({ row,c });
				break;
			}
		}
			possibleMoves.push_back({ row,c });
		
	}

	//check down
	for (int row = r+1; row < 8; row++)
	{
		if (this->m_boardScript->m_board[row][c]) {
			if ((this->m_boardScript->m_board)[row][c]->getColor() == this->m_color)
				break;
			//can capture
			if (this->m_boardScript->m_board[row][c]->getColor() != this->m_color)
			{
				possibleMoves.push_back({ row,c });
				break;
			}
		}
		possibleMoves.push_back({ row,c });
	}

	//check right
	for (int column = c+1; column < 8; column++)
	{
		if (this->m_boardScript->m_board[r][column]) {
			if (this->m_boardScript->m_board[r][column]->getColor() == this->m_color)
				break;
			//can capture
			if (this->m_boardScript->m_board[r][column]->getColor() != this->m_color)
			{
				possibleMoves.push_back({ r,column });
				break;
			}
		}
		possibleMoves.push_back({ r,column });
	}

	//check left
	for (int column = c-1; column >= 0; column--)
	{
		if (this->m_boardScript->m_board[r][column])
		{
			if (this->m_boardScript->m_board[r][column]->getColor() == this->m_color)
				break;
			//can capture
			if (this->m_boardScript->m_board[r][column]->getColor() != this->m_color)
			{
				possibleMoves.push_back({ r,column });
				break;
			}
		}
		possibleMoves.push_back({ r,column });
	}

	return possibleMoves;
}

std::vector<BoardPosition> Bishop::computePossibleMoves()
{
	std::vector<BoardPosition> possibleMoves;
	auto [r, c] = this->getPosition();

	for (int row = r - 1, column = c + 1; (row >= 0) && (column < 8); row--, column++) { //up-right
		if (this->m_boardScript->m_board[row][column]) { //there is a piece in the square
			if (this->m_boardScript->m_board[row][column]->getColor() == this->getColor()) //cannot capture same color
				break;
			else { //can capture opposite color
				possibleMoves.push_back({ row,column });
				break;
			}
		}
		possibleMoves.push_back({ row,column });
	}

	for (int row = r - 1, column = c - 1; (row >= 0) && (column >= 0); row--, column--) { //up-left
		if (this->m_boardScript->m_board[row][column]) { //there is a piece in the square
			if (this->m_boardScript->m_board[row][column]->getColor() == this->getColor()) //cannot capture same color
				break;
			else { //can capture opposite color
				possibleMoves.push_back({ row,column });
				break;
			}
		}
		possibleMoves.push_back({ row,column });
	}

	for (int row = r + 1, column = c - 1; (row < 8) && (column >= 0); row++, column--) { //bottom-left
		if (this->m_boardScript->m_board[row][column]) { //there is a piece in the square
			if (this->m_boardScript->m_board[row][column]->getColor() == this->getColor()) //cannot capture same color
				break;
			else { //can capture opposite color
				possibleMoves.push_back({ row,column });
				break;
			}
		}
		possibleMoves.push_back({ row,column });
	}

	for (int row = r + 1, column = c + 1; (row < 8) && (column < 8); row++, column++) { //bottom-right
		if (this->m_boardScript->m_board[row][column]) { //there is a piece in the square
			if (this->m_boardScript->m_board[row][column]->getColor() == this->getColor()) //cannot capture same color
				break;
			else { //can capture opposite color
				possibleMoves.push_back({ row,column });
				break;
			}
		}
		possibleMoves.push_back({ row,column });
	}

	return possibleMoves;
}

std::vector<BoardPosition> Queen::computePossibleMoves()
{
	std::vector<BoardPosition> possibleMoves;
	auto [r, c] = this->getPosition();

	for (int row = r - 1; row >= 0; row--)
	{
		if (this->m_boardScript->m_board[row][c]) {
			//cannot capture color
			if (this->m_boardScript->m_board[row][c]->getColor() == this->m_color)
				break;
			//can capture
			if (this->m_boardScript->m_board[row][c]->getColor() != this->m_color)
			{
				possibleMoves.push_back({ row,c });
				break;
			}
		}
		possibleMoves.push_back({ row,c });

	}

	//check down
	for (int row = r + 1; row < 8; row++)
	{
		if (this->m_boardScript->m_board[row][c]) {
			if ((this->m_boardScript->m_board)[row][c]->getColor() == this->m_color)
				break;
			//can capture
			if (this->m_boardScript->m_board[row][c]->getColor() != this->m_color)
			{
				possibleMoves.push_back({ row,c });
				break;
			}
		}
		possibleMoves.push_back({ row,c });
	}

	//check right
	for (int column = c + 1; column < 8; column++)
	{
		if (this->m_boardScript->m_board[r][column]) {
			if (this->m_boardScript->m_board[r][column]->getColor() == this->m_color)
				break;
			//can capture
			if (this->m_boardScript->m_board[r][column]->getColor() != this->m_color)
			{
				possibleMoves.push_back({ r,column });
				break;
			}
		}
		possibleMoves.push_back({ r,column });
	}

	//check left
	for (int column = c - 1; column >= 0; column--)
	{
		if (this->m_boardScript->m_board[r][column])
		{
			if (this->m_boardScript->m_board[r][column]->getColor() == this->m_color)
				break;
			//can capture
			if (this->m_boardScript->m_board[r][column]->getColor() != this->m_color)
			{
				possibleMoves.push_back({ r,column });
				break;
			}
		}
		possibleMoves.push_back({ r,column });
	}


	for (int row = r - 1, column = c + 1; (row >= 0) && (column < 8); row--, column++) { //up-right
		if (this->m_boardScript->m_board[row][column]) { //there is a piece in the square
			if (this->m_boardScript->m_board[row][column]->getColor() == this->getColor()) //cannot capture same color
				break;
			else { //can capture opposite color
				possibleMoves.push_back({ row,column });
				break;
			}
		}
		possibleMoves.push_back({ row,column });
	}

	for (int row = r - 1, column = c - 1; (row >= 0) && (column >= 0); row--, column--) { //up-left
		if (this->m_boardScript->m_board[row][column]) { //there is a piece in the square
			if (this->m_boardScript->m_board[row][column]->getColor() == this->getColor()) //cannot capture same color
				break;
			else { //can capture opposite color
				possibleMoves.push_back({ row,column });
				break;
			}
		}
		possibleMoves.push_back({ row,column });
	}

	for (int row = r + 1, column = c - 1; (row < 8) && (column >= 0); row++, column--) { //bottom-left
		if (this->m_boardScript->m_board[row][column]) { //there is a piece in the square
			if (this->m_boardScript->m_board[row][column]->getColor() == this->getColor()) //cannot capture same color
				break;
			else { //can capture opposite color
				possibleMoves.push_back({ row,column });
				break;
			}
		}
		possibleMoves.push_back({ row,column });
	}

	for (int row = r + 1, column = c + 1; (row < 8) && (column < 8); row++, column++) { //bottom-right
		if (this->m_boardScript->m_board[row][column]) { //there is a piece in the square
			if (this->m_boardScript->m_board[row][column]->getColor() == this->getColor()) //cannot capture same color
				break;
			else { //can capture opposite color
				possibleMoves.push_back({ row,column });
				break;
			}
		}
		possibleMoves.push_back({ row,column });
	}

	return possibleMoves;
}

std::vector<BoardPosition> King::computePossibleMoves()
{
	std::vector<BoardPosition> possibleMoves;
	auto [r, c] = this->getPosition();

	if (r - 1 >= 0)  //check up
		if ((this->m_boardScript->m_board[r - 1][c] == nullptr) || (this->m_boardScript->m_board[r - 1][c]->getColor() != this->m_color))
			possibleMoves.push_back({r-1,c});

	if (r - 1 >= 0 && c +1 <= 7)  //check up-right
		if ((this->m_boardScript->m_board[r - 1][c + 1] == nullptr) || (this->m_boardScript->m_board[r - 1][c + 1]->getColor() != this->m_color))
			possibleMoves.push_back({ r - 1,c + 1 });

	if (r - 1 >= 0 && c - 1 >= 0)  //check up-left
		if ((this->m_boardScript->m_board[r - 1][c - 1] == nullptr) || (this->m_boardScript->m_board[r - 1][c - 1]->getColor() != this->m_color))
			possibleMoves.push_back({ r - 1,c - 1 });

	if (c + 1 <= 7)  //check right
		if ((this->m_boardScript->m_board[r][c+1] == nullptr) || (this->m_boardScript->m_board[r][c+1]->getColor() != this->m_color))
			possibleMoves.push_back({ r,c +1});

	if (c - 1 >= 0)  //check left
		if ((this->m_boardScript->m_board[r][c - 1] == nullptr) || (this->m_boardScript->m_board[r][c - 1]->getColor() != this->m_color))
			possibleMoves.push_back({ r,c - 1});

	if (r + 1 <= 7)  //check down
		if ((this->m_boardScript->m_board[r + 1][c] == nullptr) || (this->m_boardScript->m_board[r + 1][c]->getColor() != this->m_color))
			possibleMoves.push_back({ r + 1,c });

	if (r + 1 <= 7 && c + 1 <= 7)  //check down-right
		if ((this->m_boardScript->m_board[r + 1][c + 1] == nullptr) || (this->m_boardScript->m_board[r + 1][c + 1]->getColor() != this->m_color))
			possibleMoves.push_back({ r + 1,c + 1 });

	if (r + 1 <= 7 && c - 1 >= 0)  //check down-left
		if ((this->m_boardScript->m_board[r + 1][c - 1] == nullptr) || (this->m_boardScript->m_board[r + 1][c - 1]->getColor() != this->m_color))
			possibleMoves.push_back({ r + 1,c - 1 });



	return possibleMoves;
}
