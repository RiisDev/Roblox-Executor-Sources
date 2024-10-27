import { Group, Text } from "@mantine/core";
import { FC, ReactNode, useCallback, useEffect, useState } from "react";

type Props = {
  endTime: number;
  extraContent?: ReactNode;
  finishedContent?: ReactNode;
};

export const Countdown: FC<Props> = ({
  endTime,
  extraContent,
  finishedContent,
}) => {
  const [content, setContent] = useState<string>();

  const newText = useCallback(() => {
    // Calculate remaining time
    const remaining = endTime - Date.now();

    // Covert to days
    const days = remaining / (1000 * 60 * 60 * 24);

    if (days > 1) return `${Math.floor(days)} Day${days === 1 ? "" : "s"}`;
    else
      return [
        (remaining / (1000 * 60 * 60)) % 24,
        (remaining / (1000 * 60)) % 60,
        (remaining / 1000) % 60,
      ]
        .map((x) =>
          Math.sign(x) !== 1 ? 0 : ("0" + Math.floor(x).toFixed(0)).slice(-2)
        )
        .join(":");
  }, [endTime]);

  useEffect(() => {
    const interval = setInterval(() => setContent(newText()), 1000);

    return () => clearInterval(interval);
  }, [endTime, newText]);

  return (
    <Text>
      <Group spacing={5}>
        {endTime < new Date().getTime() ? finishedContent ?? "🎉" : content}{" "}
        {endTime < new Date().getTime() ? null : extraContent}
      </Group>
    </Text>
  );
};
