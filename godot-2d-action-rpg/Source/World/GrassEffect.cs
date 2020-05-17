using Godot;
using System;

public class GrassEffect : Node2D
{
    private AnimatedSprite _animatedSprite;
    public override void _Ready()
    {
        _animatedSprite = GetNode<AnimatedSprite>("AnimatedSprite");
        _animatedSprite.Frame = 0;
        _animatedSprite.Play("Animate");
    }

    public override void _Process(float delta)
    {}

    private void _on_AnimatedSprite_animation_finished()
    {
        QueueFree();
    }

}
