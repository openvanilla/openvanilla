VERSION 5.00
Begin {C62A69F0-16DC-11CE-9E98-00AA00574A4F} POJForm 
   Caption         =   "OpenVanilla POJ"
   ClientHeight    =   1845
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   8655
   OleObjectBlob   =   "POJForm.frx":0000
   StartUpPosition =   1  '©ÒÄÝµøµ¡¤¤¥¡
End
Attribute VB_Name = "POJForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub keybuf_KeyUp(ByVal KeyCode As MSForms.ReturnInteger, ByVal Shift As Integer)
    k = KeyCode
    If k >= 65 And k <= 90 And Shift = 0 Then k = k + 32
    Select Case KeyCode
        Case 33         ' ovkPageUp
            k = 11
        Case 34         ' ovkPageDown
            k = 12
        Case 35         ' ovkEnd
            k = 4
        Case 36         ' ovkHome
            k = 1
        Case 37         ' ovkLeft
            k = 28
        Case 38         ' ovkUp
            k = 30
        Case 39         ' ovkRight
            k = 29
        Case 40         ' ovkDown
            k = 31
        Case 46         ' ovkDelete
            k = 127
        Case 189        ' -
            k = 45
    End Select
    
    ' action.Caption = action.Caption + Str$(k) + "," + Str$(Shift) + " "
    
    a$ = String$(1024, " ")
    rlen = KEYEVENT(k, a$)
    ' action.Caption = Mid$(a$, 1, rlen)
    
    ' parse the strings
    Dim result$(100)
    n = 0
    ln = 1
    For i = 1 To rlen
        c$ = Mid$(a$, i, 1)
        If c$ = " " Or i = rlen Then
            n = n + 1
            dst = i - 1
            If i = rlen Then dst = i
            result$(n) = Mid$(a$, ln, dst - ln + 1)
            ln = i + 1
        End If
    Next
    
    ' action.Caption = ""
    ' For i = 1 To n
    '    action.Caption = action.Caption + Str$(i) + " : " + result$(i)
    ' Next i
    
    For i = 1 To n
        x$ = result$(i)
        
        Select Case x$
            Case "bufclear"
                keybuf.Text = ""
            Case "bufupdate"
                i = i + 1
                keybuf.Text = decode(result$(i))
            Case "bufsend"
                i = i + 1
                keybuf.Text = ""
                ' committed.Caption = committed.Caption + decode(result$(i))
                Selection.TypeText Text:=decode(result$(i))
            Case "candiclear"
                candi.Caption = ""
            Case "candiupdate"
                i = i + 1
                candi.Caption = decode(result$(i))
            Case "unprocessed"
                ' committed.Caption = committed.Caption + Chr$(k)
                If k = 27 Then
                    POJForm.Hide
                    
                    
                End If
                Selection.TypeText Text:=Chr$(k)
        End Select
    Next
    
End Sub
