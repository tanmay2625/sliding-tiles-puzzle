# Reference for GUI : https://inventwithpython.com/pygame/chapter4.html

import pygame
import sys
import random
from pygame.locals import *

BOARDHEIGHT, BOARDWIDTH = map(int, input().split())
TILESIZE = 80
WINDOWWIDTH = 640
WINDOWHEIGHT = 640
FPS = 30
BLANK = None
STEPS = 0

BLACK = (0,   0,   0)
WHITE = (255, 255, 255)
BRIGHTBLUE = (0,  50, 255)
DARKTURQUOISE = (3,  54,  73)
GREEN = (0, 204,   0)

BGCOLOR = DARKTURQUOISE
TILECOLOR = BLACK
TEXTCOLOR = WHITE
BORDERCOLOR = GREEN
BASICFONTSIZE = 20

BUTTONCOLOR = BLACK
BUTTONTEXTCOLOR = WHITE
MESSAGECOLOR = WHITE

XMARGIN = int((WINDOWWIDTH - (TILESIZE * BOARDWIDTH + (BOARDWIDTH - 1))) / 2)
YMARGIN = int(
    (WINDOWHEIGHT - (TILESIZE * BOARDHEIGHT + (BOARDHEIGHT - 1))) / 2)

DOWN = 'U'
UP = 'D'
LEFT = 'R'
RIGHT = 'L'


def main():
    global FPSCLOCK, DISPLAYSURF, BASICFONT, SOLVE_SURF, SOLVE_RECT, STEPS

    pygame.init()
    FPSCLOCK = pygame.time.Clock()
    DISPLAYSURF = pygame.display.set_mode((WINDOWWIDTH, WINDOWHEIGHT))
    pygame.display.set_caption('Slide Puzzle')
    BASICFONT = pygame.font.Font('freesansbold.ttf', BASICFONTSIZE)

    # Store the option buttons and their rectangles in OPTIONS.
    SOLVE_SURF, SOLVE_RECT = makeText(
        'Solve',    TEXTCOLOR, TILECOLOR, WINDOWWIDTH - 120, WINDOWHEIGHT - 30)

    board = []
    for x in range(BOARDHEIGHT):
        column = list(map(int, input().split()))
        board.append(column)
        for y in range(BOARDWIDTH):
            if board[x][y] == 0:
                board[x][y] = BLANK
    mainBoard = []
    for y in range(BOARDWIDTH):
        tmp = []
        for x in range(BOARDHEIGHT):
            tmp.append(board[x][y])
        mainBoard.append(tmp)
    drawBoard(mainBoard, '')
    path = input()
    STEPS = len(path)
    solutionSeq = []
    for x in path:
        solutionSeq.append(x)
    
    SOLVEDBOARD = mainBoard
    allMoves = []  

    while True:  
        slideTo = None 
        msg = 'Click tile or press arrow keys to slide.'
        if mainBoard == SOLVEDBOARD:
            msg = 'Solved!'

        drawBoard(mainBoard, msg)

        checkForQuit()
        for event in pygame.event.get():  
            if event.type == MOUSEBUTTONUP:
                resetAnimation(mainBoard, solutionSeq + allMoves)
                allMoves = []

        if slideTo:
            slideAnimation(mainBoard, slideTo,
                           'Click tile or press arrow keys to slide.', 8)
            makeMove(mainBoard, slideTo)
            allMoves.append(slideTo)  # record the slide
        pygame.display.update()
        FPSCLOCK.tick(FPS)


def terminate():
    pygame.quit()
    sys.exit()


def checkForQuit():
    for event in pygame.event.get(QUIT): 
        terminate() 
    for event in pygame.event.get(KEYUP):  
        if event.key == K_ESCAPE:
            terminate()  
        pygame.event.post(event)  



def getBlankPosition(board):
    for x in range(BOARDWIDTH):
        for y in range(BOARDHEIGHT):
            if board[x][y] == BLANK:
                return (x, y)


def makeMove(board, move):
    
    blankx, blanky = getBlankPosition(board)

    if move == UP:
        board[blankx][blanky], board[blankx][blanky +
                                             1] = board[blankx][blanky + 1], board[blankx][blanky]
    elif move == DOWN:
        board[blankx][blanky], board[blankx][blanky -
                                             1] = board[blankx][blanky - 1], board[blankx][blanky]
    elif move == LEFT:
        board[blankx][blanky], board[blankx +
                                     1][blanky] = board[blankx + 1][blanky], board[blankx][blanky]
    elif move == RIGHT:
        board[blankx][blanky], board[blankx -
                                     1][blanky] = board[blankx - 1][blanky], board[blankx][blanky]


def getLeftTopOfTile(tileX, tileY):
    left = XMARGIN + (tileX * TILESIZE) + (tileX - 1)
    top = YMARGIN + (tileY * TILESIZE) + (tileY - 1)
    return (left, top)


def drawTile(tilex, tiley, number, adjx=0, adjy=0):
    left, top = getLeftTopOfTile(tilex, tiley)
    pygame.draw.rect(DISPLAYSURF, TILECOLOR,
                     (left + adjx, top + adjy, TILESIZE, TILESIZE))
    textSurf = BASICFONT.render(str(number), True, TEXTCOLOR)
    textRect = textSurf.get_rect()
    textRect.center = left + int(TILESIZE / 2) + \
        adjx, top + int(TILESIZE / 2) + adjy
    DISPLAYSURF.blit(textSurf, textRect)


def makeText(text, color, bgcolor, top, left):
    textSurf = BASICFONT.render(text, True, color, bgcolor)
    textRect = textSurf.get_rect()
    textRect.topleft = (top, left)
    return (textSurf, textRect)


def drawBoard(board, message):
    DISPLAYSURF.fill(BGCOLOR)
    if message:
        textSurf, textRect = makeText(message, MESSAGECOLOR, BGCOLOR, 5, 5)
        DISPLAYSURF.blit(textSurf, textRect)

    for tilex in range(len(board)):
        for tiley in range(len(board[0])):
            if board[tilex][tiley]:
                drawTile(tilex, tiley, board[tilex][tiley])

    left, top = getLeftTopOfTile(0, 0)
    width = BOARDWIDTH * TILESIZE
    height = BOARDHEIGHT * TILESIZE
    pygame.draw.rect(DISPLAYSURF, BORDERCOLOR, (left - 5,
                                                top - 5, width + 11, height + 11), 4)

    DISPLAYSURF.blit(SOLVE_SURF, SOLVE_RECT)


def slideAnimation(board, direction, message, animationSpeed):
    blankx, blanky = getBlankPosition(board)
    movex, movey = -1, -1
    if direction == UP:
        movex = blankx
        movey = blanky + 1
    elif direction == DOWN:
        movex = blankx
        movey = blanky - 1
    elif direction == LEFT:
        movex = blankx + 1
        movey = blanky
    elif direction == RIGHT:
        movex = blankx - 1
        movey = blanky

    drawBoard(board, message)
    baseSurf = DISPLAYSURF.copy()
    moveLeft, moveTop = getLeftTopOfTile(movex, movey)
    pygame.draw.rect(baseSurf, BGCOLOR, (moveLeft,
                                         moveTop, TILESIZE, TILESIZE))

    for i in range(0, TILESIZE, animationSpeed):
        # animate the tile sliding over
        checkForQuit()
        DISPLAYSURF.blit(baseSurf, (0, 0))
        if direction == UP:
            drawTile(movex, movey, board[movex][movey], 0, -i)
        if direction == DOWN:
            drawTile(movex, movey, board[movex][movey], 0, i)
        if direction == LEFT:
            drawTile(movex, movey, board[movex][movey], -i, 0)
        if direction == RIGHT:
            drawTile(movex, movey, board[movex][movey], i, 0)

        pygame.display.update()
        FPSCLOCK.tick(FPS)


def resetAnimation(board, allMoves):
    for move in allMoves:
        k = STEPS//10
        if STEPS < 100:
            k = 10
        slideAnimation(board, move, '', animationSpeed=k)
        makeMove(board, move)


if __name__ == '__main__':
    main()
