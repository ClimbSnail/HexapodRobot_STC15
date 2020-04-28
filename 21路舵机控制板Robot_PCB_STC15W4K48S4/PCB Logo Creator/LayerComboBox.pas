{......................................................................................................................}
Var gv_ListBoxIndexToLayerArray : TList;
{......................................................................................................................}

{......................................................................................................................}
Function GetLayerFromComboBox(ComboBox : TComboBox; Board : IPCB_Board) : TLayer;
Begin
    Result := eTopLayer;

    If ComboBox.ItemIndex < 0 Then
        Exit;

    If ComboBox.ItemIndex >= ComboBox.Items.Count Then
        Exit;

    Result := gv_ListBoxIndexToLayerArray.Items(ComboBox.ItemIndex);
End;
{......................................................................................................................}

{......................................................................................................................}
Procedure SetupComboBoxFromLayer(ComboBox : TComboBox; Board : IPCB_Board);
Var
    Layer : TLayer;
    LayerObject : IPCB_LayerObject;
Begin
    gv_ListBoxIndexToLayerArray := TList.Create;
    ComboBox.Items.Clear;

    For Layer := MinLayer To MaxLayer Do
    Begin
        If Board.LayerIsDisplayed[Layer] Then
        Begin
           gv_ListBoxIndexToLayerArray.Add(Layer);

           LayerObject := Board.LayerStack.LayerObject(Layer);
           ComboBox.Items.Add(LayerObject.Name);

           If Layer = Board.CurrentLayer Then
              ComboBox.ItemIndex := ComboBox.Items.Count - 1;
        End;
    End;
End;
{......................................................................................................................}

