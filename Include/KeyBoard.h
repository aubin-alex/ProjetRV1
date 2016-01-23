/*
 * KeyBoard.h
 *
 *  Created on: Jan 10, 2014
 *      Author: fanzhang
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

class myKeyboardEventHandler: public osgGA::GUIEventHandler {
//Initialize function
public:
	myKeyboardEventHandler() {
		_mx = 0;
		_my = 0;
	}
//Handler
	virtual bool handle(const osgGA::GUIEventAdapter& ea,
			osgGA::GUIActionAdapter& aa);
	//virtual void accept(osgGA::GUIEventHandlerVisitor& v) {
		//v.visit(*this);
	//};
//Protected objects
private:
	float _mx, _my;
};


#endif /* KEYBOARD_H_ */
