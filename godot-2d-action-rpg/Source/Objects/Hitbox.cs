using Godot;
using System;

public class Hitbox : Area2D
{
    [Export] public int Damage = 1;

    public override void _Ready()
    {}

    public override void _Process(float delta)
    {}
}
