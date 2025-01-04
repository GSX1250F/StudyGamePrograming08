Imports OpenTK.Mathematics

Public Class MoveComponent
    Inherits Component
    'public:
    Sub New(ByRef owner As Actor, ByVal updateOrder As Integer)
        MyBase.New(owner, updateOrder)
        mVelocity = Vector3.Zero
        mRotSpeed = Vector3.Zero
        mMass = 1.0
        mForce = Vector3.Zero
        mRotForce = Vector3.Zero
        mResist = 0.0
        mRotResist = 0.0
    End Sub
    Public Overrides Sub Update(deltaTime As Double)
        If (mVelocity.Length > 0.001) Then
            mOwner.SetPosition(mOwner.GetPosition() + mVelocity * deltaTime)
        End If
        If (mRotSpeed.Length > 0.001) Then
            Dim rot As Quaternion = mOwner.GetRotation
            Dim axis As Vector3 = mRotSpeed
            axis.Normalize()
            Dim angle As Double = deltaTime * mRotSpeed.Length  '角度変化量
            Dim inc = Quaternion.FromAxisAngle(axis, angle)
            'もとのrotと増分のクォータニオンを結合
            rot = Quaternion.Multiply(rot, inc)
            mOwner.SetRotation(rot)
        End If
        '速度と角速度を更新
        SetVelocity(mVelocity + GetAccel() * deltaTime)     'v = v0 + at
        SetRotSpeed(mRotSpeed + GetRotAccel() * deltaTime)     'ω = ωo + bt
    End Sub
    Public Sub SetMass(ByVal v As Double)
        mMass = v
    End Sub
    Public Sub SetMoveResist(ByVal v As Double)
        mResist = v
    End Sub
    Public Sub SetRotResist(ByVal v As Double)
        mRotResist = v
    End Sub
    Public Sub SetVelocity(ByVal v As Vector3)
        mVelocity = v
    End Sub
    Public Sub SetRotSpeed(ByVal v As Vector3)
        mRotSpeed = v
    End Sub
    Public Sub SetForce(ByVal v As Vector3)
        mForce = v
    End Sub
    Public Sub SetRotForce(ByVal v As Vector3)
        mRotForce = v
    End Sub
    Public Function GetVelocity() As Vector3
        Return mVelocity
    End Function
    Public Function GetRotSpeed() As Vector3
        Return mRotSpeed
    End Function
    Public Function GetAccel() As Vector3


        If mMass > 0.001 Then
            Dim accel As Vector3 = mForce * (1 / mMass)    '重心加速度の計算　F=ma  a=F*(1/m)
            accel -= mVelocity * mResist * 0.01 * (1 / mMass)
            Return accel
        Else
            Return Vector3.Zero
        End If
    End Function
    Public Function GetRotAccel() As Vector3
        If GetImoment() > 0.001 Then
            '回転加速度の計算　回転加速度 = トルク / 慣性モーメント
            Dim accel = GetTorque() * (1.0F / GetImoment())    '回転加速度の計算 Fr=Ia  a=Fr/I
            '抵抗力 = 速さ*抵抗係数    減速 = -速さ*抵抗係数*半径/慣性モーメント
            accel -= mRotSpeed * mOwner.GetRadius() * mRotResist * (1.0 / GetImoment())
            Return accel
        Else
            Return Vector3.Zero
        End If
    End Function
    Public Function GetImoment() As Double
        ' 慣性モーメント計算　※※2次元においては、一様密度の円板とする。 I=0.5*質量*半径^2
        Return 0.5 * mMass * mOwner.GetRadius() * mOwner.GetRadius()
    End Function
    Public Function GetTorque() As Vector3
        'トルク計算　トルク=回転方向の力 * 半径
        Return mRotForce * mOwner.GetRadius()
    End Function

    'private:
    Private mMass As Double         '質量
    Private mResist As Double       '重心速度抵抗率(%)
    Private mRotResist As Double    '回転速度抵抗率(%)

    'ワールド座標系
    Private mVelocity As Vector3     '速度
    Private mRotSpeed As Vector3     '回転速度モーメントベクトル
    Private mForce As Vector3        '重心にかかる力
    Private mRotForce As Vector3     '回転方向の力モーメントベクトル
End Class