# -*- coding: utf-8 -*-

import smtplib
import email
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.utils import parseaddr, formataddr
from email.header import Header



ser = smtplib.SMTP_SSL("smtp.ym.163.com",465)
ser.login("contact@hkaxj.com","123456")

#创建一个带附件的实例
message = MIMEMultipart()
message['From'] = Header("香港新疆联谊会", 'utf-8')
message['To'] =  Header(" ", 'utf-8')
subject = '香港新疆联谊会会员资格确认函'
message['Subject'] = Header(subject, 'utf-8')

print("connect to mail server success.")


def SendMail(x):
    print(x["<<<name>>>"])
    # 构造附件1，传送当前目录下的 test.txt 文件
    #邮件正文内容
    message.attach(MIMEText(x["<<<name>>>"]+' 您好,欢迎加入香港新疆联谊会.', 'plain', 'utf-8'))
    att1 = MIMEText(open(x["<<<name>>>"]+'.pdf', 'rb').read(), 'base64', 'utf-8')
    att1["Content-Type"] = 'application/octet-stream'
    # 这里的filename可以任意写，写什么名字，邮件中显示什么名字
    att1["Content-Disposition"] = 'attachment; filename="香港新疆联谊会会员确认函.'+x["<<<name>>>"]+'.pdf"'
    message.attach(att1)
    ser.sendmail("contact@hkaxj.com", x["<<<email>>>"], message.as_string())
    print(x["<<<email>>>"]+"确认函发送成功.")




