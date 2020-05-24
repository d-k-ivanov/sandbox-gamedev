using Godot;
using System;

public class WanderController : Node2D
{
    [Export] public int WanderRange = 32;

    private Vector2 _startPosition;
    public Vector2 TargetPosition;

    private Timer _timer;

    public override void _Ready()
    {
        _startPosition     = GlobalPosition;
        TargetPosition     = GlobalPosition;
        _timer             = GetNode<Timer>("Timer");
        UpdateTargetPosition();
    }

    public override void _PhysicsProcess(float delta)
    {}

    private void UpdateTargetPosition()
    {
        var target_vector = new Vector2(
            (float) GD.RandRange(-WanderRange, WanderRange),
            (float) GD.RandRange(-WanderRange, WanderRange));
        TargetPosition = _startPosition + target_vector;
    }

    public float GetTimeLeft()
    {
        return _timer.TimeLeft;
    }

    public void StartWanderTimer(float duration)
    {
        _timer.Start(duration);
    }

    private void _on_Timer_timeout()
    {
        UpdateTargetPosition();
    }

}
