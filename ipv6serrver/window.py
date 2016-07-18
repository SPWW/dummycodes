#!/usr/bin/python
# -*- coding: UTF-8 -*-

from tkinter import *
from v6 import holdserver
from v6udp import holdserverUdp

class Application(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.pack()
        self.createWidgets()
        self.tcp_guard = 0
        self.udp_guard = 0
		
    def createWidgets(self):
        self.lb = Label(self, text='enter port:') 
        self.lb.pack()
        self.nameInput = Entry(self,text='port')
        self.nameInput.pack()
        self.alertButton1 = Button(self, text='Tcp server start', command=self.tcp)
        self.alertButton1.pack()
        self.alertButton2 = Button(self, text='Udp server start', command=self.udp)
        self.alertButton2.pack()

    def tcp(self):
        name = self.nameInput.get() or "10008"
        self.alertButton1.configure(bg = "red")
        holdserver(name)
		
    def udp(self):
        name = self.nameInput.get() or "10008"
        self.alertButton2.configure(bg = "red")
        holdserverUdp(name)
        
        
if __name__ == "__main__":   
	app = Application()
	app.master.title('Socket Listener')
	app.master.geometry('250x100')
	app.master.resizable(width=False,height=False)
	app.mainloop()
