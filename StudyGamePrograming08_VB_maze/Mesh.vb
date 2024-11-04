Imports System.IO
Imports System.Net
Imports System.Runtime.ConstrainedExecution
Imports OpenTK.Mathematics

Public Class Mesh
	Implements IDisposable      '明示的にクラスを開放するために必要
	'public
	Public Sub New()
		mRadius = 0.0
		mSpecPower = 100.0
	End Sub
	Public Overloads Sub Dispose() Implements IDisposable.Dispose
		Dispose(True)
	End Sub
	Protected disposed = False     '開放処理が実施済みかのフラグ
	Protected Overridable Overloads Sub Dispose(ByVal disposing As Boolean)
		If Not Me.disposed Then
			If disposing Then
				'*** アンマネージリソースの開放
			End If
			'*** マネージドリソースの開放
		End If
		disposed = True
	End Sub
	Protected Overrides Sub Finalize()
		MyBase.Finalize()
		Dispose(False)
	End Sub
	Public Function Load(ByRef fileName As String, ByRef renderer As Renderer)
		If Not System.IO.File.Exists(fileName) Then
			Console.WriteLine("メッシュファイル " & fileName & " が存在しません。")
			Return False
		Else
			'すべてのテキストを1つの文字列に読み込む
			Dim jsonText As String = File.ReadAllText(fileName)
			Dim doc As Newtonsoft.Json.Linq.JObject = DirectCast(Newtonsoft.Json.JsonConvert.DeserializeObject(jsonText), Newtonsoft.Json.Linq.JObject)
			If doc Is Nothing Then
				Console.WriteLine("メッシュファイル " & fileName & " は有効なJSONファイルではありません。")
				Return False
			End If
			Dim ver As Integer = doc("version")
			If (ver <> 1) Then
				Console.WriteLine("メッシュファイル " & fileName & " のバージョンが1ではありません。")
				Return False
			End If
			mShaderName = doc("shader").ToString

			Dim vertSize As Integer = 8

			' テクスチャを読み込み
			Dim textures = doc("textures").ToArray
			If (textures.Count < 1) Then
				Console.WriteLine("メッシュファイル " & fileName & " にテクスチャが見つかりません。最低でも１つ必要です。")
				Return False
			End If
			mSpecPower = Convert.ToSingle(doc("specularPower"))
			For i As Integer = 0 To textures.Count - 1
				'このテクスチャがすでに読み込まれたか？
				Dim texName As String = textures(i).ToString
				Dim t As Texture = renderer.GetTexture(texName)
				If (t Is Nothing) Then
					'テクスチャ読み込みを試行
					t = renderer.GetTexture(texName)
					If (t Is Nothing) Then
						'Nullのままの場合、デフォルトのテクスチャを使用する。
						t = renderer.GetTexture("Assets/Default.png")
					End If
				End If
				mTextures.Add(t)
			Next
			'頂点配列群をロード
			Dim vertsJson = doc("vertices")
			If (vertsJson.Count < 1) Then
				Console.WriteLine("メッシュファイル " & fileName & " が頂点配列群を持っていません")
				Return False
			End If
			Dim vertices = New List(Of Single)
			mRadius = 0.0
			For i As Integer = 0 To vertsJson.Count - 1
				'頂点配列群は8個の要素を持つ
				Dim vert = vertsJson(i)
				If vert.Count() <> 8 Then
					Console.WriteLine(fileName & " が想定外の頂点配列フォーマットです。")
					Return False
				End If
				Dim pos = New Vector3(Convert.ToSingle(vert(0)), Convert.ToSingle(vert(1)), Convert.ToSingle(vert(2)))
				mRadius = Math.Max(mRadius, pos.LengthSquared)
				For j As Integer = 0 To vert.Count - 1
					vertices.Add(Convert.ToSingle(vert(j)))
				Next
			Next

			mRadius = Math.Sqrt(mRadius)

			'インデックス群をロード
			Dim indJson = doc("indices")
			If indJson.Count < 1 Then
				Console.WriteLine("メッシュファイル " & fileName & " がインデックス群を持っていません")
				Return False
			End If

			Dim indices = New List(Of UInteger)
			For i As Integer = 0 To indJson.Count - 1
				Dim ind = indJson(i)
				If ind.Count <> 3 Then
					Console.WriteLine(fileName & " が想定外のインデックスフォーマットです。")
					Return False
				End If
				indices.Add(Convert.ToUInt32(ind(0)))
				indices.Add(Convert.ToUInt32(ind(1)))
				indices.Add(Convert.ToUInt32(ind(2)))
			Next

			'頂点配列を作成
			mVertexInfo = New VertexInfo(vertices.ToArray, Convert.ToUInt32(vertices.Count / vertSize), indices.ToArray, indices.Count)
			Return True
		End If

	End Function
    Public Sub Unload()
		mVertexInfo.Dispose()
		mVertexInfo = Nothing
	End Sub
	Public Function GetVertexInfo() As VertexInfo
		Return mVertexInfo
	End Function
	Public Function GetTexture(ByVal index As Integer) As Texture
		If (index < mTextures.Count) Then
			Return mTextures(index)
		Else
			Return Nothing
		End If
	End Function

	Public Function GetShaderName() As String
        Return mShaderName
    End Function
	Public Function GetRadius() As Single
		Return mRadius
	End Function
	Public Function GetSpecPower() As Single
		Return mSpecPower
	End Function


	'private
	Private mTextures As New List(Of Texture)
	Private mVertexInfo As VertexInfo
    Private mShaderName As String
	Private mRadius As Single
	Private mSpecPower As Single
End Class
