Attribute VB_Name = "POJTest"
Public Declare Function KEYEVENT Lib "C:\OVVBPOJ\OVIMPOJ.DLL" (ByVal key As Long, ByVal action As String) As Long


Function dehex(x$) As Long
    Y = Asc(x$)
    If Y >= 48 And Y <= 57 Then
        dehex = Y - 48
    Else
        dehex = Y - 97 + 10
    End If
    
End Function

Function deutf16(x$) As Long
    deutf16 = dehex(Mid$(x$, 1, 1)) * 4096 + dehex(Mid$(x$, 2, 1)) * 256 + dehex(Mid$(x$, 3, 1)) * 16 + dehex(Mid$(x$, 4, 1))
    
End Function

Function decode(x$) As String
    l = Len(x$)
    
    For i = 0 To (l / 4) - 1
        Y$ = Y$ + ChrW$(deutf16(Mid$(x$, i * 4 + 1, 4)))
    Next
    decode = Y$
End Function


Sub ShowPOJ_stub()
    POJForm1.candi = ""
    POJForm1.Show
    
End Sub

