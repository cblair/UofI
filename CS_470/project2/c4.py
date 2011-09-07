#! /usr/bin/python

import pygame,sys,os
import numpy
from pygame.locals import *


global_rowpx = global_colpx  = global_bannerpx = 100
global_window = pygame.display.set_mode((7*global_colpx,
					(6*global_rowpx) + global_bannerpx)) 

global_pg = pygame
pygame.init()
pygame.display.set_caption('Connect 4')
global_screen = pygame.display.get_surface()
piece_fname = os.path.join("./images/","empty.jpg")
global_empty_piece_surface = pygame.image.load(piece_fname)
piece_fname = os.path.join("./images/","red.jpg")
global_red_piece_surface = pygame.image.load(piece_fname)	
piece_fname = os.path.join("./images/","black.jpg")
global_black_piece_surface = pygame.image.load(piece_fname)
boardbg_fname = os.path.join("./images/","bg.jpg")
global_surface = pygame.image.load(boardbg_fname)


class c4:
    def __init__(self):
	#general attributes
	self.rowpx = self.colpx  = self.bannerpx = 100
	self.turn = 0 #whos turn it is
	self.sim = False #sim mode for ai, True = don't draw()

	#game init
        #self.pg = pygame
        #self.pg.init()
        self.pg = global_pg
	#window = self.pg.display.set_mode((7*self.colpx,
	#					(6*self.rowpx) + self.bannerpx))
	window = global_window
        #self.pg.display.set_caption('Connect 4')
        #self.screen = self.pg.display.get_surface()
	self.screen = global_screen

	#setup board bg
	self.board = numpy.zeros((7,6), dtype=int) #std dims of connect 4
	self.xmax = self.board.shape[0]
	self.ymax = self.board.shape[1]
	self.surface = global_surface
	#self.screen.blit(self.surface, (0,0))
        #self.pg.display.flip()        

	#set up pieces
	(self.empty, self.red, self.black) = range(0,3)
        self.empty_piece_surface = global_empty_piece_surface
        self.red_piece_surface = global_red_piece_surface	
        self.black_piece_surface = global_black_piece_surface


    def copy_board(self):
	retb = numpy.zeros((7,6), dtype=int)
	for x in range(0,self.xmax):
		for y in range(0,self.ymax):
			retb[x][y] = self.board[x][y]
	return retb

    def avail_colrow(self,colnum):
	for y in range(self.ymax - 1, -1, -1):
		if self.board[colnum][y] == self.empty:
			return y
		if y == 0:
			return -1

    def is_board_full(self):
	for y in range(0,self.ymax): #y
		for x in range(0,self.xmax):
			if self.board[x][y] == self.empty:
				return False
	return True
	
    def draw(self,opt_text=""):
	#draw board
	if self.sim:
		return
	for y in range(0,self.ymax): #y
		for x in range(0,self.xmax):
			#print "|%d|" % (self.board[x][y]),
			if self.board[x][y] == self.empty:
				piece = self.empty_piece_surface
			elif self.board[x][y] == self.red:
				piece = self.red_piece_surface
			elif self.board[x][y] == self.black:
				piece = self.black_piece_surface
			#else things will be bad. 
		        self.screen.blit(piece, (x*self.colpx,y*self.rowpx))
		#print
	#draw banner
   	if self.is_winner():
		text = "Player %d WINS! Play again? (y/n)" % (self.turn + 1)
	elif self.is_board_full():
		text = "Draw! Play again? (y/n)"
	else:
		text = "Player %d's turn. %s" % (self.turn + 1, opt_text)
	font = self.pg.font.Font(None,36)
	text = font.render(text, 1, (10, 10, 10))
	self.screen.blit(self.surface, (0,(self.ymax*self.rowpx)))
	self.screen.blit(text, (0,(self.ymax*self.rowpx)))
	self.pg.display.flip()

    def print_board(self,opt_text=""):
	#print board
	for y in range(0,self.ymax): #y
		for x in range(0,self.xmax):
			print "|%d|" % (self.board[x][y]),
		print

    def is_winner(self):
	#check each col
	for x in range(0,self.xmax):
		for y in range(0,self.ymax - 3):
			piece = self.board[x][y]
			if piece == self.empty:
				next
			elif (self.board[x][y+1] == piece
				and self.board[x][y+2] == piece
				and self.board[x][y+3] == piece
				):
				return True
	#check each row
	for y in range(0,self.ymax):
		for x in range(0,self.xmax - 3):
			piece = self.board[x][y]
			if piece == self.empty:
				next
			elif (self.board[x+1][y] == piece
				and self.board[x+2][y] == piece
				and self.board[x+3][y] == piece
				):
				return True
	#check each diag - neg slope
	for x in range(0,self.xmax - 3):
		for y in range(0,self.ymax - 3):
			piece = self.board[x][y]
			if piece == self.empty:
				next
			elif (self.board[x+1][y+1] == piece
				and self.board[x+2][y+2] == piece
				and self.board[x+3][y+3] == piece
				):
				return True
	#check each diag - pos slope
	for x in range(0,self.xmax - 3):
		for y in range(4,self.ymax):
			piece = self.board[x][y]
			if piece == self.empty:
				next
			elif (self.board[x+1][y-1] == piece
				and self.board[x+2][y-2] == piece
				and self.board[x+3][y-3] == piece
				):
				return True
	return False

    def play_piece(self,colnum):
	#colnum = pos[0] / self.colpx
	avail_colrow = self.avail_colrow(colnum)
	if avail_colrow == -1:
		font = self.pg.font.Font(None,36)
		text = 'Bad Move'
		self.draw(text)
		return False
	if self.turn == 0:
		self.board[colnum][avail_colrow] = self.red
	else:
		self.board[colnum][avail_colrow] = self.black
	return True
	
    def play(self,mode):
	from c4search import c4search
	self.mode = mode
	search = c4search()
	self.draw()
	while(True):
	        while not(self.is_winner() or self.is_board_full()):
        	    for event in self.pg.event.get():
                	#print event
	                if (self.mode != "2player" and self.turn == 1):
				#ai mode
				if (self.play_piece(search.best_move(self,0))
					and not(self.is_winner())):
					self.turn = (self.turn + 1) % 2
				self.draw()
			elif event.type == MOUSEBUTTONDOWN:
				#1 player play, ai or 2player mode
				if (self.play_piece(event.pos[0] / self.colpx)
					and not(self.is_winner())):
					self.turn = (self.turn + 1) % 2
				self.draw()
		while True:
			for event in self.pg.event.get():
				if event.type == KEYDOWN:
					if event.key == 121: #y
						return True
					elif event.key == 110: #n
						return False

class c4_2:
    def __init__(self):
	#general attributes
	self.rowpx = self.colpx  = self.bannerpx = 100
	self.turn = 0 #whos turn it is

	#game init
        self.pg = pygame
        self.pg.init()
       	#window = self.pg.display.set_mode((7*self.colpx,
	#					(6*self.rowpx) + self.bannerpx))
	window = global_window
        self.pg.display.set_caption('Connect 4')
        self.screen = self.pg.display.get_surface()

	#setup board bg
	self.board = numpy.zeros((7,6), dtype=int) #std dims of connect 4
	self.xmax = self.board.shape[0]
	self.ymax = self.board.shape[1]
	self.surface = global_surface
	#self.screen.blit(self.surface, (0,0))
        #self.pg.display.flip()        

	#set up pieces
	(self.empty, self.red, self.black) = range(0,3)
        self.empty_piece_surface = global_empty_piece_surface
        self.red_piece_surface = global_red_piece_surface	
        self.black_piece_surface = global_black_piece_surface


			
