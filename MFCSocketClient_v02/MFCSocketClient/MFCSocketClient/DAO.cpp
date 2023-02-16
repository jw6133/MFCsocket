#include "pch.h"
#include "DAO.h"

CString msgCString;

CString DAO::getMsg()
{
    return msgCString;
}

void DAO::setMsg(CString msg)
{
    msgCString = msg;
}


