from graphics import *
class Pixel:
    def __init__(self,og,size):
        self.og = og
        self.size = size
        self._color = "white"
        self.rect = Rectangle(og,Point(self.og.getX()+size,self.og.getY()+size))
        self.rect.setFill(self._color)
    
    @property
    def color(self):
        return self._color
    
    
    @color.setter
    def color(self, new_color):
        self._color = new_color
        self.rect.setFill(self._color)

    def draw(self,thiswin):
        self.rect.draw(thiswin)

class Colon:
    def __init__(self,og,pxSize):
        self.og = og
        self.spacing = pxSize #set the spacing between half of the size
        self.pxSize = pxSize
        self.top=Pixel(Point(self.og.getX(),self.og.getY()-self.pxSize-self.spacing/2),self.pxSize)
        self.bottom=Pixel(Point(self.og.getX(),self.og.getY()+self.spacing/2),self.pxSize)
        self.junk = Circle(og,2)
        self.junk.color = "red"

    @property
    def color(self):
        return self._color
    
    
    @color.setter
    def color(self, new_color):
        self._color = new_color
        self.top.color = self._color
        self.bottom.color = self._color
    
    def draw(self,thiswin):
        self.top.draw(thiswin)
        self.bottom.draw(thiswin)
        self.junk.draw(thiswin)

class Segment:
    def __init__(self,og,angle,pxSize, numPix):
        self.og = og
        self.angle = angle
        self.pxSize = pxSize
        self.pixels = []
        self._color = "white"
        self.numPix = numPix
        for i in range(self.numPix):
            location = Point(self.og.getX(),self.og.getY()+self.pxSize*i)
            self.pixels.append(Pixel(location,self.pxSize))

    @property
    def color(self):
        return self._color
    
    
    @color.setter
    def color(self, new_color):
        self._color = new_color
        for i in range(self.numPix):
            self.pixels[i].color = self._color

    
    def draw(self,thiswin):
        for i in range(self.numPix):
            self.pixels[i].draw(thiswin)




winSizeX = 1000
winSizeY = 500
pixelSize = 20
pixelsPerSegment = 3
win = GraphWin("Display",winSizeX,winSizeY)
win.setCoords(winSizeX, winSizeY, 0, 0)



segment = Segment(Point(200,200),0,pixelSize,pixelsPerSegment)
segment.draw(win)
win.getMouse()
segment.color= "purple"
colon = Colon(Point(200,300),pixelSize)
colon.color = "red"
colon.draw(win)
win.getMouse()
colon.color = "blue"
win.getMouse()
win.close()

