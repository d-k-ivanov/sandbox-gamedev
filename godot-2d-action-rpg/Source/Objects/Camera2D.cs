using Godot;
using System;

public class Camera2D : Godot.Camera2D
{
    private Position2D _topLeftLimit;
    private Position2D _bottomRightLimit;

    public override void _Ready()
    {
        _topLeftLimit     = GetNode<Position2D>("Limits/TopLeft");
        _bottomRightLimit = GetNode<Position2D>("Limits/BottomRight");

        LimitTop    = (int) _topLeftLimit.GlobalPosition.y;
        LimitLeft   = (int) _topLeftLimit.GlobalPosition.x;
        LimitBottom = (int) _bottomRightLimit.GlobalPosition.y;
        LimitRight  = (int) _bottomRightLimit.GlobalPosition.x;
    }
}
