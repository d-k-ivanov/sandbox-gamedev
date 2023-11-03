using Godot;
using System;

public partial class Camera2D : Godot.Camera2D
{
	private Marker2D _topLeftLimit;
	private Marker2D _bottomRightLimit;

	public override void _Ready()
	{
		_topLeftLimit     = GetNode<Marker2D>("Limits/TopLeft");
		_bottomRightLimit = GetNode<Marker2D>("Limits/BottomRight");

		LimitTop    = (int) _topLeftLimit.GlobalPosition.Y;
		LimitLeft   = (int) _topLeftLimit.GlobalPosition.X;
		LimitBottom = (int) _bottomRightLimit.GlobalPosition.Y;
		LimitRight  = (int) _bottomRightLimit.GlobalPosition.X;
	}
}
