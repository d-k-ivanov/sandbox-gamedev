using Godot;
using System;

public partial class PlayerDetectionZone : Area2D
{
    public Player Player;

    public override void _Ready()
    {
        Player = null;
    }

    public override void _Process(double delta)
    {}

    public bool IsPlayerDetected()
    {
        return Player != null;
    }

    private void _on_PlayerDetectionZone_body_entered(Player body)
    {
        Player = body;
    }

    private void _on_PlayerDetectionZone_body_exited(Player body)
    {
        Player = null;
    }
}


