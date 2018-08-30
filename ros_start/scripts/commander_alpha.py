#!/usr/bin/env python
# -*- coding:utf-8 -*-

import rospy
from std_msgs.msg import Int32
from geometry_msgs.msg import Point
from std_msgs.msg import Float32
from time import sleep

class Alpha(object):
    def __init__(self):                    #topic名:  a=>alpha, b=>beta, m=>motor
        self._instruct = rospy.Publisher('/atom/cmd_instruct', Point, queue_size=10)
        self._talk = rospy.Publisher('/atob/cmd_conv', Point, queue_size=10)
        self._wait = rospy.Publisher('/atom/cmd_wait', Point, queue_size=10)
        self._start = rospy.Subscriber('cmd_start', Int32, self.start_callback, queue_size=10)
        self._order = rospy.Subscriber('/mtoa/cmd_order', Point, self.order_callback, queue_size=10)
        self._listen = rospy.Subscriber('/btoa/cmd_conv', Point, self.conv_callback, queue_size=10)
        self._info = Point() #x = flag, y = color
        #self._wait = Int32()

    def start_callback(self, start_msg): #スタートコマンドを受け取った時の処理
        point = Point()
        if start_msg.data == 1:  
            print "do fg = 0"
            fg = 0
            point.x = fg
            point.y = 0
            self._instruct.publish(point)

    def order_callback(self, order_msg):             #モータ側のarduinoからフラッグと色情報を受け取った時の処理
        #wait_msg = Int32()
        if order_msg.x != self._info.x:              #同じ動きをしないかのチェック(同じ行動で衝突を防ぐため)
           if order_msg.x == 1:
              print "do fg = 1"
              self._instruct.publish(order_msg)      #モータ側に送信
              self._talk.publish(order_msg)          #ベータ（もう一つのraspi）に送信

           elif order_msg.x == 2:
              if self._info.x != 4:                    #外側優
                 if order_msg.y != self._info.y:        #同じ色のボールを持っているか否かのチェック(同じゴールで衝突を防ぐため)
                    print "do fg = 2"
                    self._instruct.publish(order_msg)   #モータ側に送信
                    self._talk.publish(order_msg)       #ベータ（もう一つのraspi）に送信                 
                 else:               
                     print "dwait!!"
                     #wait_msg.data = 1
                     self._wait.publish(order_msg)        #同じ行動をするようであるなら、モータ側に待機を命令

              else:
                     print "wait!!"
                     #wait_msg.data = 1
                     self._wait.publish(order_msg)

           elif order_msg.x == 3:
              print "do fg = 3"
              self._instruct.publish(order_msg)      #モータ側に送信
              self._talk.publish(order_msg)          #ベータ（もう一つのraspi）に送信

           elif order_msg.x == 4:
              if self._info.x != 4:                    #外側優
                 if order_msg.y != self._info.y:        #同じボールを持っているか否かのチェック(同じゴールで衝突を防ぐため)
                    print "do fg = 4"
                    self._instruct.publish(order_msg)   #モータ側に送信
                    self._talk.publish(order_msg)       #ベータ（もう一つのraspi）に送信
                 else:                                 
                    print "wait!!"
                    #wait_msg.data = 1
                    self._wait.publish(order_msg)        #同じ行動をするようであるなら、モータ側に待機を命令
              else:
                 print "wait"
                 #wait_msg.data = 1
                 self._wait.publish(order_msg)


        else:               
           print "wait!!"                
           #wait_msg.data = 1
           self._wait.publish(order_msg)              #同じ行動をするようであるなら、モータ側に待機を命令
           
           
    def conv_callback(self, conv_msg):
        print "conversation!!"
        self._info.x = conv_msg.x                    #どの行動状態にいるか
        self._info.y = conv_msg.y                    #どのボールを持っているか

if __name__ == '__main__':
    rospy.init_node('alpha')
    alpha = Alpha()
    rospy.spin()
