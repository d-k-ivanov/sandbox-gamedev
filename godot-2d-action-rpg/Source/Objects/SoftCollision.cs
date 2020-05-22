using Godot;
using System;

public class SoftCollision : Area2D
{
    public override void _Ready()
    {}

    public override void _Process(float delta)
    {}

    public bool IsColliding()
    {
        var areas = GetOverlappingAreas();
        return areas.Count > 0;
    }

    public Vector2 GetPushVector()
    {
        var areas = GetOverlappingAreas();
        var pushVector = Vector2.Zero;
        if (IsColliding())
        {
            var area = (Area2D) areas[0];
            if (area != null) pushVector = area.GlobalPosition.DirectionTo(GlobalPosition);
            pushVector = pushVector.Normalized();
        }

        return pushVector;
    }
}
